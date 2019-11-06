#include "DataManager.h"
#include "VoxelBuilder.h"

#define	FILEHEADER "VXT.File"

bool Data::SaveVoxelData(World& world, const char* filePath) {
	std::ofstream file(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
	if(!file.is_open())
		return false;

	file.write(FILEHEADER, strlen(FILEHEADER));

	int size = sizeof(float);
	glm::vec3 col, pos;

	std::vector<Voxel*> voxels = std::vector<Voxel*>();
	world.GetAllVoxels(voxels);

	for(auto const& voxel : voxels) {
		col = voxel->colour;
		pos = voxel->position;
	
		// Colour
		file.write((char*)& col.r, size);
		file.write((char*)& col.g, size);
		file.write((char*)& col.b, size);
	
		// Position
		file.write((char*)& pos.x, size);
		file.write((char*)& pos.y, size);
		file.write((char*)& pos.z, size);
	}

	file.close();

	return true;
}

Data::Handler Data::LoadVoxelData(World& world, const char* filePath) {
	std::ifstream file(filePath, std::ios::in | std::ios::binary);
	if(!file.is_open())
		return Data::ERROR_NO_FILE;

	int headerSize = strlen(FILEHEADER);
	char* header = new char[headerSize + 1]{ '\0' };

	// Check VXT file header
	file.read((char*)header, headerSize);
	if(strcmp(FILEHEADER, header) != 0) {
		file.close();
		delete[] header;
		return Data::ERROR_CORRUPTED;
	}

	float* buffer = new float[6] { 0.0f };

	std::vector<Voxel> voxels = std::vector<Voxel>();
	Voxel voxel;
	while(file.peek() != EOF) {
		file.read((char*)buffer, 6 * sizeof(float));
		if((file.rdstate() & std::ifstream::failbit) != 0 || (file.rdstate() & std::ifstream::badbit)) { 
			file.close(); 
			delete[] buffer; 
			return Data::ERROR_CORRUPTED; 
		}

		voxel = Voxel();
		for(int i = 0; i < 6; i++) {
			if(i < 3)
				voxel.colour[i] = buffer[i];
			else
				voxel.position[i - 3] = buffer[i];
		}

		//data.push_back(voxel);
		voxels.push_back(voxel);
	}

	world.DestroyWorld();
	world.AddBlocks(voxels);

	delete[] header;
	delete[] buffer;
	return Data::SUCCESS;
}

Data::Handler Data::VoxelExport(World& world, const char* filePath) {
	COLLADACompiler out = COLLADACompiler();

	out.CompileWorldData(world);
	bool successful = out.Compile(filePath);

	if(successful)
		return Data::SUCCESS;
	else
		return Data::ERROR_CORRUPTED;
}

#pragma region COLLADA Compiler



COLLADACompiler::COLLADACompiler() {
	this->positions = "";
	this->triangles = "";
	this->colours = "";
	this->verticesCount = 0;
	this->total = 0;
	this->usedColours = std::vector<glm::vec3>();
}

void COLLADACompiler::CompileWorldData(World& world) { // const std::map<int, Voxel>& data
	std::vector<Voxel*> voxels = std::vector<Voxel*>();
	bool left = false, front = false, right = false, back = false, up = false, down = false;
	glm::vec3 pos;

	world.GetAllVoxels(voxels);

	for(auto const& voxel : voxels) {
		pos = voxel->position;

		// Find neighbouring voxels
		right	= (world.HasNeighbour(*voxel, Chunk::Direction::EAST));
		left	= (world.HasNeighbour(*voxel, Chunk::Direction::WEST));
		up		= (world.HasNeighbour(*voxel, Chunk::Direction::UP));
		down	= (world.HasNeighbour(*voxel, Chunk::Direction::DOWN));
		front	= (world.HasNeighbour(*voxel, Chunk::Direction::SOUTH));
		back	= (world.HasNeighbour(*voxel, Chunk::Direction::NORTH));

		// Reverse X & Z
		pos.x = -pos.x;
		pos.z = -pos.z;

		// Add colour
		int colourIndex = AddColour(voxel->colour);

		// 5 6 2  5 2 1
		if(!right) {
			AddVecToString(pos + glm::vec3(-0.5f, -0.5f, -0.5f), this->positions);	// BOT RIGHT  5 = 0
			AddVecToString(pos + glm::vec3(-0.5f, -0.5f, 0.5f), this->positions);	// TOP RIGHT  6 = 1
			AddVecToString(pos + glm::vec3(-0.5f, 0.5f, 0.5f), this->positions);	// TOP RIGHT  2 = 2 
			AddVecToString(pos + glm::vec3(-0.5f, 0.5f, -0.5f), this->positions);	// BOT RIGHT  1 = 3

			CalculateIndex(colourIndex, 1);
			verticesCount += 12;
		}

		// 740 703
		if(!left) {
			AddVecToString(pos + glm::vec3(0.5f, -0.5f, 0.5f), this->positions); // TOP LEFT   7
			AddVecToString(pos + glm::vec3(0.5f, -0.5f, -0.5f), this->positions); // BOT LEFT   4
			AddVecToString(pos + glm::vec3(0.5f, 0.5f, -0.5f), this->positions); // BOT LEFT	0
			AddVecToString(pos + glm::vec3(0.5f, 0.5f, 0.5f), this->positions); // TOP LEFT   3
		
			CalculateIndex(colourIndex, 0);
			verticesCount += 12;
		}

		// 0 1 2  0 2 3
		if(!up) {
			AddVecToString(pos + glm::vec3(0.5f, 0.5f, -0.5f), this->positions); // BOT LEFT	0
			AddVecToString(pos + glm::vec3(-0.5f, 0.5f, -0.5f), this->positions); // BOT RIGHT  1
			AddVecToString(pos + glm::vec3(-0.5f, 0.5f, 0.5f), this->positions); // TOP RIGHT  2
			AddVecToString(pos + glm::vec3(0.5f, 0.5f, 0.5f), this->positions); // TOP LEFT   3

			CalculateIndex(colourIndex, 4);
			verticesCount += 12;
		}

		// 4 7 6  4 6 5
		if(!down) {
			AddVecToString(pos + glm::vec3(0.5f, -0.5f, -0.5f), this->positions); // BOT LEFT   4
			AddVecToString(pos + glm::vec3(0.5f, -0.5f, 0.5f), this->positions); // TOP LEFT   7
			AddVecToString(pos + glm::vec3(-0.5f, -0.5f, 0.5f), this->positions); // TOP RIGHT  6 
			AddVecToString(pos + glm::vec3(-0.5f, -0.5f, -0.5f), this->positions); // BOT RIGHT  5

			CalculateIndex(colourIndex, 5);
			verticesCount += 12;
		}

		// 4 5 1  4 1 0
		if(!front) {
			AddVecToString(pos + glm::vec3(0.5f, -0.5f, -0.5f), this->positions); // BOT LEFT   4
			AddVecToString(pos + glm::vec3(-0.5f, -0.5f, -0.5f), this->positions); // BOT RIGHT  5
			AddVecToString(pos + glm::vec3(-0.5f, 0.5f, -0.5f), this->positions); // BOT RIGHT  1
			AddVecToString(pos + glm::vec3(0.5f, 0.5f, -0.5f), this->positions); // BOT LEFT	0

			CalculateIndex(colourIndex, 3);
			verticesCount += 12;
		}

		// 6 7 3  6 3 2
		if(!back) {
			AddVecToString(pos + glm::vec3(-0.5f, -0.5f, 0.5f), this->positions); // TOP RIGHT  6 
			AddVecToString(pos + glm::vec3(0.5f, -0.5f, 0.5f), this->positions); // TOP LEFT   7
			AddVecToString(pos + glm::vec3(0.5f, 0.5f, 0.5f), this->positions); // TOP LEFT   3
			AddVecToString(pos + glm::vec3(-0.5f, 0.5f, 0.5f), this->positions); // TOP RIGHT  2

			CalculateIndex(colourIndex, 2);
			verticesCount += 12;
		}
	}
}

void COLLADACompiler::AddVecToString(const glm::vec3& vec, std::string& string) {
	string.append(std::to_string(vec[0]) + " ");
	string.append(std::to_string(vec[1]) + " ");
	string.append(std::to_string(vec[2]) + " ");
}

int COLLADACompiler::AddColour(const glm::vec3& colour) {
	for(int i = 0; i < usedColours.size(); i++) {
		if(usedColours[i] == colour) {
			return i;
		}
	}

	usedColours.push_back(colour);
	colours.append(std::to_string(colour.r) + " ");
	colours.append(std::to_string(colour.g) + " ");
	colours.append(std::to_string(colour.b) + " ");

	return usedColours.size() - 1;
}

void COLLADACompiler::CalculateIndex(const int& colourIndex, const int& normal) {
	const int verticesPerFace = 4;
	int offset = total * verticesPerFace;
	total += 1;

	std::string c_normal = std::to_string(normal);
	std::string c_color = std::to_string(colourIndex);

	triangles.append(std::to_string(0 + offset));
	triangles.append(" " + c_normal + " ");
	triangles.append(c_color + " ");
	triangles.append(std::to_string(1 + offset));
	triangles.append(" " + c_normal + " ");
	triangles.append(c_color + " ");
	triangles.append(std::to_string(2 + offset));
	triangles.append(" " + c_normal + " ");
	triangles.append(c_color + " ");

	triangles.append(std::to_string(0 + offset));
	triangles.append(" " + c_normal + " ");
	triangles.append(c_color + " ");
	triangles.append(std::to_string(2 + offset));
	triangles.append(" " + c_normal + " ");
	triangles.append(c_color + " ");
	triangles.append(std::to_string(3 + offset));
	triangles.append(" " + c_normal + " ");
	triangles.append(c_color + " ");
}

bool COLLADACompiler::Compile(const char* path) {
	tinyxml2::XMLDocument base = new tinyxml2::XMLDocument();
	base.InsertEndChild(base.NewDeclaration());

	tinyxml2::XMLNode* elements = base.InsertEndChild(base.NewElement("COLLADA"));
	base.FirstChildElement()->SetAttribute("version", "1.4.1");

	tinyxml2::XMLNode* asset = elements->InsertEndChild(base.NewElement("asset"));
	{
		asset->InsertEndChild(base.NewElement("up_axis"));
		asset->FirstChildElement()->SetText("Y_UP");
	}

	tinyxml2::XMLNode* geometries = elements->InsertEndChild(base.NewElement("library_geometries"));
	{
		tinyxml2::XMLNode* geometry = geometries->InsertEndChild(base.NewElement("geometry"));
		geometries->FirstChildElement()->SetAttribute("id", "Cube-mesh");
		geometries->FirstChildElement()->SetAttribute("name", "Cube");

		// MESH
		tinyxml2::XMLNode* mesh = geometry->InsertEndChild(base.NewElement("mesh"));
		{
			tinyxml2::XMLNode* source = mesh->InsertEndChild(base.NewElement("source"));

			// POSITIONS/VERTCIES
			mesh->LastChildElement()->SetAttribute("id", "Cube-mesh-positions");
			{
				source->InsertEndChild(base.NewElement("float_array"));
				source->FirstChildElement()->SetAttribute("id", "Cube-mesh-positions-array");
				source->FirstChildElement()->SetAttribute("count", verticesCount);
				source->FirstChildElement()->SetText(positions.c_str());
			}

			source = mesh->InsertEndChild(base.NewElement("source"));

			// NORMALS
			mesh->LastChildElement()->SetAttribute("id", "Cube-mesh-normals");
			{
				source->InsertEndChild(base.NewElement("float_array"));
				source->FirstChildElement()->SetAttribute("id", "Cube-mesh-normals-array");
				source->FirstChildElement()->SetAttribute("count", 18);
				source->FirstChildElement()->SetText(normals);
			}

			source = mesh->InsertEndChild(base.NewElement("source"));

			// COLORS
			mesh->LastChildElement()->SetAttribute("id", "Cube-mesh-colors-Col");
			{
				source->InsertEndChild(base.NewElement("float_array"));
				source->FirstChildElement()->SetAttribute("id", "Cube-mesh-colors-Col-array");
				source->FirstChildElement()->SetAttribute("count", usedColours.size());
				source->FirstChildElement()->SetText(colours.c_str());
			}

			source = mesh->InsertEndChild(base.NewElement("vertices"));

			// VERTICIES
			mesh->LastChildElement()->SetAttribute("id", "Cube-mesh-vertices");
			{
				source->InsertEndChild(base.NewElement("input"));
				source->FirstChildElement()->SetAttribute("semantic", "POSITION");
				source->FirstChildElement()->SetAttribute("source", "#Cube-mesh-positions");
			}

			source = mesh->InsertEndChild(base.NewElement("triangles"));

			// TRIANGLES
			mesh->LastChildElement()->SetAttribute("material", "Material-material");
			mesh->LastChildElement()->SetAttribute("count", 2);
			{
				// VERTEX
				source->InsertEndChild(base.NewElement("input"));
				source->LastChildElement()->SetAttribute("semantic", "VERTEX");
				source->LastChildElement()->SetAttribute("source", "#Cube-mesh-vertices");
				source->LastChildElement()->SetAttribute("offset", 0);

				// NORMAL
				source->InsertEndChild(base.NewElement("input"));
				source->LastChildElement()->SetAttribute("semantic", "NORMAL");
				source->LastChildElement()->SetAttribute("source", "#Cube-mesh-normals");
				source->LastChildElement()->SetAttribute("offset", 1);

				// COLOR
				source->InsertEndChild(base.NewElement("input"));
				source->LastChildElement()->SetAttribute("semantic", "COLOR");
				source->LastChildElement()->SetAttribute("source", "#Cube-mesh-colors-Col");
				source->LastChildElement()->SetAttribute("offset", 2);

				// INDEXES
				source->InsertEndChild(base.NewElement("p"));
				source->LastChildElement()->SetText(triangles.c_str());
			}
		}
	}

	tinyxml2::XMLNode* visual = elements->InsertEndChild(base.NewElement("library_visual_scenes"));
	{
		tinyxml2::XMLNode* scenes = visual->InsertEndChild(base.NewElement("visual_scene"));
		{
			visual->LastChildElement()->SetAttribute("id", "Scene");
			visual->LastChildElement()->SetAttribute("name", "Scene");

			tinyxml2::XMLNode* node = scenes->InsertEndChild(base.NewElement("node"));
			{
				scenes->LastChildElement()->SetAttribute("id", "Cube");
				scenes->LastChildElement()->SetAttribute("name", "Cube");
				scenes->LastChildElement()->SetAttribute("type", "NODE");

				tinyxml2::XMLNode* matrix = node->InsertEndChild(base.NewElement("matrix"));
				{
					node->LastChildElement()->SetAttribute("sid", "transform");
					node->LastChildElement()->SetText("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1");
				}

				tinyxml2::XMLNode* instance = node->InsertEndChild(base.NewElement("instance_geometry"));
				{
					node->LastChildElement()->SetAttribute("url", "#Cube-mesh");
					node->LastChildElement()->SetAttribute("name", "Cube");
				}
			}
		}
	}

	// SCENE
	tinyxml2::XMLNode* scene = elements->InsertEndChild(base.NewElement("scene"));
	{
		scene->InsertEndChild(base.NewElement("instance_visual_scene"));
		scene->FirstChildElement()->SetAttribute("url", "#Scene");
	}

	base.SaveFile(path);
	printf("Exporting File To: %s\nStatus: %s\n", path, base.ErrorStr());

	return !base.Error();
}

#pragma endregion