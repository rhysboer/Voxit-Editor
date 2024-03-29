#include "VoxelBuilder.h"
#include "stb_image.h"

VoxelBuilder::VoxelBuilder() : history(historySize) {
	this->grid = Grid(mapSize + 1);
	this->fileHandler = FileHandler();
	this->world = new World();

	// Selector
	this->selector = Selector();
	this->selector.Init();
	this->selector.SetActive(false);
	this->isSelecting = false;
	this->currentTool = ToolIDs::PLACE;
	this->buttonStates[(unsigned int)currentTool] = true;

	this->voxelMapSize = 0;
	this->voxelColour = glm::vec3(1.0f);
	this->voxelColourSecondary = glm::vec3::vec(0.5f);

	world->InitWorld();
}

VoxelBuilder::~VoxelBuilder() {
	world->DestroyWorld();
	history.Clear();
}

void VoxelBuilder::OnUpdate() {
	DrawGUI();

	if(Settings::useAnimateLight && Settings::useLighting) {
		Settings::angle = fmod(Settings::angle + Time::DeltaTime() * Settings::rotationSpeed, 360.0f);

		float sin = glm::sin(glm::radians(Settings::angle));
		float cos = glm::cos(glm::radians(Settings::angle));
		
		glm::vec3 dVec = Settings::SUN_DIRECTION;
		
		Settings::sunDirection.x = (cos * dVec.x) + (sin * dVec.z);
		Settings::sunDirection.z = (cos * dVec.z) - (sin * dVec.x);
	}


	switch(currentTool) {
		case VoxelBuilder::ToolIDs::PLACE: {
			ToolPlace();
			break;
		}
		case VoxelBuilder::ToolIDs::COLOUR: {
			ToolEyedropper();
			break;
		}
		case VoxelBuilder::ToolIDs::ERASE: {
			ToolEraser();
			break;
		}
		case VoxelBuilder::ToolIDs::GRADIENT: {
			ToolGradient();
			break;
		}
		default: {
			currentTool = VoxelBuilder::ToolIDs::PLACE;
			ToolPlace();
		}
	}

	if(!isSelecting)
		selector.Clear();

	// UNDO (Ctrl + z)
	if(Input::IsKeyDown(GLFW_KEY_LEFT_CONTROL) && Input::IsKeyPressed(GLFW_KEY_Z)) {
		if(history.Count() > 0) {
			switch((*history.GetNewest()).type) {
				case HistoryNode::UndoType::ADD:
				{
					world->RemoveBlocks((*history.GetNewest()).voxels);
					history.Pop_Newest();
					break;
				}
				case HistoryNode::UndoType::REMOVE:
				{
					world->AddBlocks((*history.GetNewest()).voxels);
					history.Pop_Newest();

					break;
				}
			}
		}
	}
}

void VoxelBuilder::Render(Camera* camera) {
	// DRAW DEPTH MAP
	world->RenderDepthMap();

	// Render Grid
	grid.OnDraw(camera);

	// Render World
	world->Render();

	// Render selector/highlighter
	selector.OnDraw(camera);
}

#pragma region Tool Functions

void VoxelBuilder::ToolPlace() {
	if(Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_LEFT)) {
		if(this->isSelecting == false) {

			RaycastHit hit = Raycaster::TEST_VoxelDetection(*world);

			if(IsValidMapPos(hit.position)) {
				selector.SetActive(true);
				selector.SetStart(hit.position);
				isSelecting = true;
			}
		}

		RaycastHit hit = Raycaster::TEST_VoxelDetection(*world);// (&voxelMap);

		if(IsValidMapPos(hit.position)) {
			selector.SetEnd(hit.position);
		} else {
			selector.SetEnd(ClosestPosInsideMap(hit.position));
		}

	} else {
		if(isSelecting == true) { // OnMouse Up
			glm::vec3 dist = selector.GetDistance();
			glm::vec3 start = selector.GetStart();
			unsigned int size = dist.x * dist.y * dist.z;

			std::vector<Voxel> voxels = std::vector<Voxel>();

			glm::vec3 position;
			glm::vec3 offset = glm::vec3(
				(start.x > selector.GetEnd().x) ? -1.0f : 1.0f,
				(start.y > selector.GetEnd().y) ? -1.0f : 1.0f,
				(start.z > selector.GetEnd().z) ? -1.0f : 1.0f
			);
			int index = 0;
			for(int y = 0; y < dist.y; y++) {
				for(int z = 0; z < dist.z; z++) {
					for(int x = 0; x < dist.x; x++, index++) {
						position = start + (glm::vec3(x, y, z) * offset);
			
						if(IsValidMapPos(position) && world->GetVoxel(position) == nullptr) {
							Voxel voxel = Voxel();
							voxel.colour = (Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) ? voxelColourSecondary : voxelColour;
							voxel.position = position;
							voxels.push_back(voxel);
						}
					}
				}
			}

			world->AddBlocks(voxels);
			AddToHistory(voxels, HistoryNode::UndoType::ADD);
		}

		selector.SetActive(false);
		isSelecting = false;
	}
}

void VoxelBuilder::ToolEyedropper() {
	if(Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_LEFT)) {
		RaycastHit hit = Raycaster::TEST_VoxelDetection(*world);

		if(hit.hitObject == true) {
			selector.SetActive(true);
			selector.SetStart(hit.object->position);
			selector.SetEnd(hit.object->position);
			isSelecting = true;
		} else {
			selector.SetActive(false);
		}
	} else {
		if(isSelecting == true) {

			if(IsValidMapPos(selector.GetStart())) {
				Voxel* voxel = world->GetVoxel(selector.GetStart());

				if(voxel != nullptr) {
					if(Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
						voxelColourSecondary = voxel->colour;
					} else {
						voxelColour = voxel->colour;
					}
				}
			}

			isSelecting = false;
			selector.SetActive(false);
		}
	}
}

void VoxelBuilder::ToolEraser() {
	if(Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_LEFT)) {
		RaycastHit hit = Raycaster::TEST_VoxelDetection(*world);

		if(this->isSelecting == false) {
			isSelecting = true;
			selector.SetActive(true);

			if(hit.hitObject) {
				selector.SetStart(hit.object->position);
			} else {
				selector.SetStart(ClosestPosInsideMap(hit.position));
			}
		}

		if(hit.hitObject) {
			selector.SetEnd(hit.object->position);
		} else {
			selector.SetEnd(ClosestPosInsideMap(hit.position));
		}

	} else {
		if(isSelecting == true) {
			isSelecting = false;
			selector.SetActive(false);

			if(IsValidMapPos(selector.GetStart())) {
				glm::vec3 dist = selector.GetDistance();
				glm::vec3 start = selector.GetStart();
				unsigned int size = dist.x * dist.y * dist.z;

				bool anyDeletions = false;

				glm::vec3 offset = glm::vec3(
					(start.x > selector.GetEnd().x) ? -1.0f : 1.0f,
					(start.y > selector.GetEnd().y) ? -1.0f : 1.0f,
					(start.z > selector.GetEnd().z) ? -1.0f : 1.0f
				);

				int index = 0;
				std::vector<Voxel> deletedVoxels = std::vector<Voxel>();

				for(int y = 0; y < dist.y; y++) {
					for(int z = 0; z < dist.z; z++) {
						for(int x = 0; x < dist.x; x++, index++) {
							Voxel* voxel = world->GetAndRemoveVoxel(start + (glm::vec3(x, y, z) * offset));

							if(voxel != nullptr) {
								deletedVoxels.push_back(Voxel(*voxel));
								delete voxel;
								anyDeletions = true;
							}
						}
					}
				}

				if(anyDeletions) {
					AddToHistory(deletedVoxels, HistoryNode::UndoType::REMOVE);
				}
			}
		}
	}
}

void VoxelBuilder::ToolGradient() {
	if(Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_LEFT)) {
		if(this->isSelecting == false) {

			RaycastHit hit = Raycaster::TEST_VoxelDetection(*world);

			if(IsValidMapPos(hit.position)) {
				selector.SetActive(true);
				selector.SetStart(hit.position);
isSelecting = true;
			}
		}

		RaycastHit hit = Raycaster::TEST_VoxelDetection(*world);

		if(IsValidMapPos(hit.position)) {
			selector.SetEnd(hit.position);
		} else {
			selector.SetEnd(ClosestPosInsideMap(hit.position));
		}

	} else {
	if(isSelecting == true) { // OnMouse Up
		glm::vec3 dist = selector.GetDistance();
		glm::vec3 start = selector.GetStart();
		unsigned int size = dist.x * dist.y * dist.z;

		std::vector<Voxel> voxels = std::vector<Voxel>();

		glm::vec3 position;
		glm::vec3 offset = glm::vec3(
			(start.x > selector.GetEnd().x) ? -1.0f : 1.0f,
			(start.y > selector.GetEnd().y) ? -1.0f : 1.0f,
			(start.z > selector.GetEnd().z) ? -1.0f : 1.0f
		);
		int index = 0;
		float max = glm::distance(selector.GetStart(), selector.GetEnd()) + 1;
		for(int y = 0; y < dist.y; y++) {
			for(int z = 0; z < dist.z; z++) {
				for(int x = 0; x < dist.x; x++, index++) {
					position = start + (glm::vec3(x, y, z) * offset);

					if(IsValidMapPos(position) && world->GetVoxel(position) == nullptr) {
						float distance = glm::distance(start + glm::vec3(x, y, z) * offset, selector.GetEnd()) / max;

						Voxel voxel = Voxel();
						voxel.colour = glm::mix(voxelColourSecondary, voxelColour, distance);
						voxel.position = position;
						voxels.push_back(voxel);
					}
				}
			}
		}

		world->AddBlocks(voxels);
		AddToHistory(voxels, HistoryNode::UndoType::ADD);
	}

	selector.SetActive(false);
	isSelecting = false;
	}
}

#pragma endregion

void VoxelBuilder::DrawGUI() {
	// Windows
	fileHandler.ShowOpenWindow(*world);

	if(ImGui::BeginMainMenuBar(), ImGuiWindowFlags_NoBringToFrontOnFocus) {
		if(ImGui::BeginMenu("Menu")) {
			if(ImGui::MenuItem("New")) {
				// CLEAR VOXELS
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Save File")) {
				fileHandler.OpenWindow(FileHandler::Window::FILE_SAVE);
			}
			if(ImGui::MenuItem("Load File")) {
				fileHandler.OpenWindow(FileHandler::Window::FILE_OPEN);
			}
			if(ImGui::MenuItem("Export (.dae)")) {
				fileHandler.OpenWindow(FileHandler::Window::FILE_EXPORT);
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Exit")) {}
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Options")) {
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			glm::vec2 canvasSize = glm::vec2(250, 250); // Size of canvas
			glm::vec2 canvasCenter;						// Center

			if(ImGui::Checkbox("Show Grid", &Settings::useGrid))
				grid.enabled = Settings::useGrid;
			ImGui::Checkbox("Show Voxel Outline", &Settings::useVoxelOutline);
			ImGui::Checkbox("Display Highlighter Size", &Settings::useHighlighterSize);

			ImGui::Text("Background Colour (Drag)");
			ImGui::ColorEdit3("", glm::value_ptr(Settings::backgroundColour));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Lighting Settings");

			if(ImGui::Checkbox("Enable Lighting", &Settings::useLighting) && !Settings::useLighting) {
				Settings::useAnimateLight = false;
			}
			ImGui::Checkbox("Auto Rotate", &Settings::useAnimateLight);
			ImGui::Text("Rotation");

			// Rotation
			if(Settings::useAnimateLight) {
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

				ImVec4 col_bg = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
				ImVec4 col_sl = ImGui::GetStyleColorVec4(ImGuiCol_SliderGrab);

				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(col_bg.x, col_bg.y, col_bg.z, 0.3f));
				ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(col_sl.x, col_sl.y, col_sl.z, 0.3f));
			}
			if(ImGui::SliderFloat("", &Settings::angle, 0.0f, 360.0f, "%.1f")){
				float sin = glm::sin(glm::radians(Settings::angle));
				float cos = glm::cos(glm::radians(Settings::angle));

				glm::vec3 dVec = Settings::SUN_DIRECTION;

				Settings::sunDirection.x = (cos * dVec.x) + (sin * dVec.z);
				Settings::sunDirection.z = (cos * dVec.z) - (sin * dVec.x);
			}
			if(Settings::useAnimateLight) {
				ImGui::PopItemFlag();
				ImGui::PopStyleColor(2);
			}

			ImGui::Text("Rotation Speed");
			ImGui::SliderFloat("##rotateSpeed", &Settings::rotationSpeed, Settings::ROTATION_SPEED_MIN, Settings::ROTATION_SPEED_MAX);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	// EDITOR

	ImGui::Begin("Voxel Editor", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove); // | ImGuiWindowFlags_AlwaysAutoResize
	ImGui::SetWindowPos(ImVec2(Window::GetSize().x - ImGui::GetWindowWidth(), 18.0f));
	ImGui::SetWindowSize(ImVec2(280, Window::GetSize().y));
	ImGui::Text("Tools:");

	// PENCIL
	if(ImGui::ImageSelectButton((void*)Icons::GetIcon(Icons::IconID::PEN), ImVec2(32.0f, 32.0f), buttonStates[(unsigned int)VoxelBuilder::ToolIDs::PLACE]) || Input::IsKeyPressed(GLFW_KEY_B)) {
		ClearAndSetButtonState(VoxelBuilder::ToolIDs::PLACE);
	}
	ImGui::SameLine();
	if(ImGui::IsItemHovered())
		ImGui::SetTooltip("Pencil:\nPlace Voxel\nL-Click: Primary Colour\nL-Click + L-Shift: Secondary Colour\nShortcut: B");

	// EYE DROPPER
	if(ImGui::ImageSelectButton((void*)Icons::GetIcon(Icons::IconID::EYEDROPPER), ImVec2(32.0f, 32.0f), buttonStates[(unsigned int)VoxelBuilder::ToolIDs::COLOUR]) || Input::IsKeyPressed(GLFW_KEY_I)) {
		ClearAndSetButtonState(VoxelBuilder::ToolIDs::COLOUR);
	}
	ImGui::SameLine();
	if(ImGui::IsItemHovered())
		ImGui::SetTooltip("Eye Dropper:\nGet Voxel Colour\nL-Click: Primary Colour\nL-Click + L-Shift: Secondary Colour\nShortcut: I");

	// ERASER
	if(ImGui::ImageSelectButton((void*)Icons::GetIcon(Icons::IconID::ERASER), ImVec2(32.0f, 32.0f), buttonStates[(unsigned int)VoxelBuilder::ToolIDs::ERASE]) || Input::IsKeyPressed(GLFW_KEY_F)) {
		ClearAndSetButtonState(VoxelBuilder::ToolIDs::ERASE);
	}
	ImGui::SameLine();
	if(ImGui::IsItemHovered())
		ImGui::SetTooltip("Eraser:\nRemove Voxel\nShortcut: F");

	// GRADIENT
	if(ImGui::ImageSelectButton((void*)Icons::GetIcon(Icons::IconID::GRADIENT), ImVec2(32.0f, 32.0f), buttonStates[(unsigned int)VoxelBuilder::ToolIDs::GRADIENT]) || Input::IsKeyPressed(GLFW_KEY_G)) {
		ClearAndSetButtonState(VoxelBuilder::ToolIDs::GRADIENT);
	}
	if(ImGui::IsItemHovered())
		ImGui::SetTooltip("Gradient:\nLerp Between Two Colours\nShortcut: G");

	// PRIMARY COLOUR
	ImGui::Text("Primary Colour:");
	ImGui::ColorPicker3("##0", glm::value_ptr(voxelColour));

	// SECONDARY COLOUR
	ImGui::Text("Secondary Colour:");
	ImGui::ColorPicker3("##1", glm::value_ptr(voxelColourSecondary));

	// STATS
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	glm::vec3 cameraPos = Camera::ActiveCamera->Position();

	ImGui::Text("\tStatistics");
	ImGui::Text("FPS: %.1f", 1.0f / Time::DeltaTime());
	ImGui::Text("Total Voxels: %i", world->TotalVoxels());
	ImGui::Text("Camera Pos: %.1f, %.1f, %.1f", cameraPos.x, cameraPos.y, cameraPos.z);

	glm::vec3 distance = selector.GetDistance();
	ImGui::Spacing();
	ImGui::Text("\tHighlighter");
	ImGui::Text("Start Pos: %.1f, %.1f, %.1f", selector.GetStart().x, selector.GetStart().y, selector.GetStart().z);
	ImGui::Text("End Pos: %.1f, %.1f, %.1f", selector.GetEnd().x, selector.GetEnd().y, selector.GetEnd().z);
	ImGui::Text("Size: %.1f, %.1f, %.1f", distance.x, distance.y, distance.z);

	ImGui::End();

	// Mouse Highlighter Size
	if(isSelecting && Settings::useHighlighterSize) {
		ImVec2 pos = ImGui::GetMousePos();
		ImGui::SetNextWindowPos(ImVec2(pos.x + 8, pos.y + 5));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
		ImGui::Begin("COORDINATES", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Size");
		ImGui::Text("X: %.1f, Y: %.1f, Z: %.1f", distance.x, distance.y, distance.z);
		ImGui::End();
		ImGui::PopStyleColor(1);
	}
}

void VoxelBuilder::ClearAndSetButtonState(ToolIDs set) {
	for(int i = 0; i < (unsigned int)VoxelBuilder::ToolIDs::COUNT; i++)
		buttonStates[i] = false;
	buttonStates[(unsigned int)set] = true;
	currentTool = set;

	this->isSelecting = false;
}

void VoxelBuilder::AddToHistory(std::vector<Voxel>& voxels, HistoryNode::UndoType type) {
	std::shared_ptr<HistoryNode> node = std::make_shared<HistoryNode>();

	for(int i = 0; i < voxels.size(); i++) 
		node->voxels.push_back(Voxel(voxels[i]));

	node->type = type;
	history.Push(node);
}

#pragma region Static Functions

int VoxelBuilder::PositionToKey(glm::vec3 position) {
	return	((position.y + (mapSize / 2)) * (mapSize * mapSize)) +
			((position.z + (mapSize / 2)) * mapSize) +
			(position.x + (mapSize / 2));
}

glm::vec3 VoxelBuilder::ConvertPosToClosestMapPos(glm::vec3 pos) {
	return glm::vec3(round(pos.x), glm::clamp(round(pos.y), 0.0f, (float)mapSize), round(pos.z));
}

glm::vec3 VoxelBuilder::ClosestPosInsideMap(glm::vec3 pos) {
	glm::vec3 max = glm::vec3((mapSize - 1) / 2, mapSize, (mapSize - 1) / 2);
	return glm::vec3((pos.x > max.x) ? max.x : (pos.x < -max.x) ? -max.x : pos.x,
					 (pos.y > max.y) ? max.y : (pos.y < 0.0f) ? 0.0f : pos.y,
					 (pos.z > max.z) ? max.z : (pos.z < -max.z) ? -max.z : pos.z);
}

bool VoxelBuilder::IsValidMapPos(glm::vec3 pos) {
	if(pos.x < (-mapSize + 1) / 2 || pos.x >(mapSize - 1) / 2) return false;
	if(pos.y < 0 || pos.y >(mapSize - 1)) return false;
	if(pos.z < (-mapSize + 1) / 2 || pos.z >(mapSize - 1) / 2) return false;
	return true;
}

#pragma endregion