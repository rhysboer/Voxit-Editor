#include "FileHandler.h"

#define VOXEL_FILE_EXT ".vxt"
#define COLLADA_FILE_EXT ".dae"
#define CURRENT_DIRECTORY "."
#define	PARENT_DIRECTORY ".."

FileHandler::FileHandler() {
	this->currentOpen = FileHandler::Window::NONE;

	this->selectedFileName = "";
	this->currentDirectory = "C:\\";
	this->directoryBar = this->currentDirectory;
	this->hasSearched = false;
	this->hasMessage = false;
}

FileHandler::~FileHandler() {
}

void FileHandler::OpenWindow(FileHandler::Window window) {
	if(window != currentOpen)
		hasSearched = false;
	currentOpen = window;
}

void FileHandler::ShowOpenWindow(World& world) {
	switch(currentOpen) {
		case FileHandler::Window::FILE_OPEN:	LoadFileDialog(world); break;
		case FileHandler::Window::FILE_SAVE:	SaveFileDialog(world); break;
		case FileHandler::Window::FILE_EXPORT:	ExportFileDialog(world); break;
	}
}

bool FileHandler::IsWindowOpen() const {
	return (currentOpen == Window::NONE) ? true : false;
}

bool FileHandler::LoadFileDialog(World& world) {
	bool wasSuccessful = false;

	ShowMessageBox();

	ImGui::Begin("Load File", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	// File explorer
	Explorer(VOXEL_FILE_EXT);

	ImGui::Text("File Name (%s)", VOXEL_FILE_EXT);
	ImGui::InputText("##hide2", &selectedFileName, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();

	ImGui::Dummy(ImVec2(60, 0)); ImGui::SameLine();

	if(ImGui::Button("Refresh", ImVec2(100, 19))) {
		hasSearched = false;
	}
	ImGui::SameLine();

	if(ImGui::Button("Open", ImVec2(100, 19))) {
		switch(Data::LoadVoxelData(world, (currentDirectory + selectedFileName).data())) {
			case Data::SUCCESS: 
				wasSuccessful = true;
				SetPopupMessage("File loaded successfully!", MessageBoxType::SUCCESS);
				break;
			case Data::ERROR_CORRUPTED:
				SetPopupMessage("File is corrupted!", MessageBoxType::ERROR_);
				break;
			case Data::ERROR_NO_FILE:
				SetPopupMessage("No file selected!", MessageBoxType::ERROR_);
				break;
		}
	}
	ImGui::SameLine();
	
	if(ImGui::Button("Close", ImVec2(100, 19))) {
		ImGui::End();
		hasSearched = false;
		currentOpen = Window::NONE;
		return wasSuccessful;
	}

	ImGui::End();
	return wasSuccessful;
}

bool FileHandler::SaveFileDialog(World& world) {
	ShowMessageBox();

	ImGui::Begin("Save File", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	// File explorer
	Explorer(VOXEL_FILE_EXT);

	ImGui::Text("File Name (%s)", VOXEL_FILE_EXT);
	ImGui::InputText("##hide2", &selectedFileName); ImGui::SameLine();

	ImGui::Dummy(ImVec2(60, 0)); ImGui::SameLine();

	if(ImGui::Button("Refresh", ImVec2(100, 19))) {
		hasSearched = false;
	}
	ImGui::SameLine();

	// SAVE FILE
	if(ImGui::Button("Save", ImVec2(100, 19)) && !hasMessage) {
		if(selectedFileName.size() <= 0) {
			SetPopupMessage("No file selected!", MessageBoxType::ERROR_);
		} else {
			if(!HasFileExtension(selectedFileName.data(), VOXEL_FILE_EXT)) {
				selectedFileName.append(VOXEL_FILE_EXT);
			}

			if(Data::SaveVoxelData(world, (currentDirectory + selectedFileName).data())) {
				SetPopupMessage("Successfully saved file!", MessageBoxType::SUCCESS);
				hasSearched = false;
			} else {
				SetPopupMessage(("Unable to save file: " + currentDirectory + selectedFileName).data(), MessageBoxType::ERROR_);
			}
		}
	}
	
	ImGui::SameLine();
	if(ImGui::Button("Close", ImVec2(100, 19)) && !hasMessage) {
		ImGui::End();
		hasSearched = false;
		currentOpen = Window::NONE;
		return false;
	}

	ImGui::End();

	return false;
}

bool FileHandler::ExportFileDialog(World& world) {
	ShowMessageBox();

	ImGui::Begin("Export File", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	// File explorer
	Explorer(COLLADA_FILE_EXT);

	ImGui::Text("File Name (%s)", COLLADA_FILE_EXT);
	ImGui::InputText("##hide2", &selectedFileName); ImGui::SameLine();

	ImGui::Dummy(ImVec2(60, 0)); ImGui::SameLine();

	if(ImGui::Button("Refresh", ImVec2(100, 19))) {
		hasSearched = false;
	}
	ImGui::SameLine();

	// EXPORT FILE
	if(ImGui::Button("Save", ImVec2(100, 19)) && !hasMessage) {
		if(selectedFileName.size() <= 0) {
			SetPopupMessage("No file selected!", MessageBoxType::ERROR_);
		} else {
			if(!HasFileExtension(selectedFileName.data(), COLLADA_FILE_EXT)) {
				selectedFileName.append(COLLADA_FILE_EXT);
			}

			Data::Handler handler = Data::VoxelExport(world, (currentDirectory + selectedFileName).data());

			if(handler == Data::SUCCESS) {
				SetPopupMessage(("Successfully exported file!\n" + currentDirectory + selectedFileName).data(), MessageBoxType::SUCCESS);
				hasSearched = false;
			} else {
				SetPopupMessage("Failed to export file!", MessageBoxType::ERROR_);
			}
		}
	}

	ImGui::SameLine();
	if(ImGui::Button("Close", ImVec2(100, 19)) && !hasMessage) {
		ImGui::End();
		hasSearched = false;
		currentOpen = Window::NONE;
		return false;
	}

	ImGui::End();

	return false;
}

void FileHandler::Explorer(const char* filter) {
	SearchDirectory(filter);

	ImGui::Text("Directory:");
	ImGui::SetNextItemWidth(600);
	if(ImGui::InputText("##hide1", &directoryBar, ImGuiInputTextFlags_EnterReturnsTrue)) {
		NextDirectory(directoryBar, true);
	}

	ImGui::BeginChild("View", ImVec2(600, 360), true);
	std::set<std::string>::iterator iter = folders.begin();
	for(; iter != folders.end(); iter++) {
		if((*iter).compare(CURRENT_DIRECTORY) == 0) continue;

		ImGui::Image((void*)Icons::GetIcon(Icons::IconID::FOLDER), ImVec2(16, 16));
		ImGui::SameLine();

		if(ImGui::Selectable((*iter).data(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
			if(ImGui::IsMouseDoubleClicked(0)) {
				NextDirectory((*iter).data(), false);
			}
		}
	}

	iter = files.begin();
	for(; iter != files.end(); iter++) {
		ImGui::Image((void*)Icons::GetIcon(Icons::IconID::FILE), ImVec2(16, 16));
		ImGui::SameLine();
		if(ImGui::Selectable((*iter).data())) {
			selectedFileName = (*iter).data();
		}
	}
	ImGui::EndChild();
}

bool FileHandler::NextDirectory(std::string directory, bool set) {
	if(directory.size() <= 0) {
		directoryBar = currentDirectory;
		return false;
	}

	std::string nextDirectory;

	// Set directory
	if(set) {
		nextDirectory = directory;
		if(nextDirectory[nextDirectory.length() - 1] != '\\') {
			printf("%c", nextDirectory[nextDirectory.length() - 1]);
			nextDirectory.append("\\");
		}
	} else if(directory.compare(PARENT_DIRECTORY) == 0) { // Go up
		nextDirectory = currentDirectory;
		// Shave off '\\*'
		nextDirectory.erase(nextDirectory.end() - 3, nextDirectory.end());
		// find last '\\'
		int pos = nextDirectory.find_last_of("\\");
		// check if its not the first '\\'
		if(pos >= 2 || pos < -1) {
			// remove everything off the last '\\'
			nextDirectory.erase(pos + 1, nextDirectory.size());
		} else {
			printf("Can't go to directory: %s", nextDirectory.data());
			return false;
		}
	} else { // Go Down
		nextDirectory = currentDirectory;
		nextDirectory += directory +"\\";
	}
	
	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFileA((nextDirectory + "*").data(), &data);
	if(handle == INVALID_HANDLE_VALUE) {
		printf("Failed to load directory: %s \nError: %i", nextDirectory.data(), GetLastError());
		directoryBar = currentDirectory;
		return false;
	}

	currentDirectory = nextDirectory;
	directoryBar = currentDirectory;
	selectedFileName = "";
	hasSearched = false;
	return true;
}

bool FileHandler::SearchDirectory(const char* filter) {
	if(hasSearched) return true;

	folders.clear();
	files.clear();

	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFileA((currentDirectory + "*").data(), &data);

	if(handle == INVALID_HANDLE_VALUE) {
		printf("Failed to load directory: %s \nError: %i", currentDirectory.data(), GetLastError());
		return false;
	}

	do {
		if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && (data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM) {	// Folder
			if(data.cFileName == ".") continue;
			folders.emplace(data.cFileName);
		} else { // File
			if(filter) {
				if(HasFileExtension(data.cFileName, filter) == true) {
					files.emplace(data.cFileName);
				}
			} else {
				files.emplace(data.cFileName);
			}
		}

	} while(FindNextFileA(handle, &data) != NULL);

	FindClose(handle);

	hasSearched = true;
	return true;
}

bool FileHandler::HasFileExtension(const char* name, const char* extension) const {
	int size = strlen(name);
	int extsize = strlen(extension);

	if(size <= 0 || extsize <= 0) return false;
	if(strncmp(name + size - extsize, extension, extsize) == 0)
		return true;
	return false;
}

void FileHandler::SetPopupMessage(const char* errorMessage, MessageBoxType type) {
	this->message = errorMessage;
	this->messageType = type;
	this->hasMessage = true;
}

void FileHandler::ShowMessageBox() {
	if(hasMessage) {
		ImGui::Begin((messageType == MessageBoxType::ERROR_) ? "Error!" : "Success!", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize(ImVec2(400, 0));
		ImGui::SetWindowFocus();

		ImGui::Image((messageType == MessageBoxType::ERROR_) ? (void*)Icons::GetIcon(Icons::IconID::ERROR_) : (void*)Icons::GetIcon(Icons::IconID::SUCCESS), ImVec2(32, 32)); ImGui::SameLine();
		ImGui::TextWrapped(message.data());
		if(ImGui::Button("Ok", ImVec2(75, 19))) {
			hasMessage = false;
		}
		ImGui::End();
	}
}
