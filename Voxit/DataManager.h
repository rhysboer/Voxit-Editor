#pragma once
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include "World.h"
#include "tinyxml2.h"

class VoxelBuilder;
namespace Data {
	enum Handler {
		SUCCESS,
		ERROR_NO_FILE,
		ERROR_CORRUPTED
	};

	bool SaveVoxelData(const char* filePath);
	Handler LoadVoxelData(const char* filePath);

	Handler VoxelExport(const char* filePath);
}

class COLLADACompiler {
public:
	COLLADACompiler();
	~COLLADACompiler() {}

	void CompileWorldData(); // const std::map<int, Voxel>& data

	// Compile file and export it
	bool Compile(const char* path);

private:

	int AddColour(const glm::vec3& colour);
	void AddVecToString(const glm::vec3& vec, std::string& string);
	void CalculateIndex(const int& colourIndex, const int& normal);

	// LEFT RIGHT | BACKWARD FORWARD | UP DOWN
	const char* normals = "1.0 0.0 0.0 -1.0 0.0 0.0 0.0 0.0 1.0 0.0 0.0 -1.0 0.0 1.0 0.0 0.0 -1.0 0.0";
		
	std::vector<glm::vec3> usedColours;
	std::string colours;
	std::string positions;
	std::string triangles;
	int verticesCount;
	int total;
};
