#include "ShaderManager.h"

std::map<const char*, Shader> ShaderManager::shaders = std::map<const char*, Shader>();

void ShaderManager::InitShaders() {
	AddShader("default");
	AddShader("line");
	AddShader("lineColour");
	AddShader("selector");
	AddShader("voxel");
	AddShader("voxelBasic");

	AddShader("screen");
}

Shader* ShaderManager::GetShader(const char* name) {
	std::map<const char*, Shader>::iterator iter = shaders.find(name);
	if(iter != shaders.end()) {
		return &(*iter).second;
	}

	printf("GetShader() : Failed to find shader: %s!", name);
	return nullptr;
}

void ShaderManager::AddShader(const char* fileNames) {
	std::string path = PATH; //"./shaders/";
	std::ifstream file;

	// Vertex
	file.open(path + fileNames + ".vert");

	if(!file.is_open()) {
		printf("Failed to load Vertex Shader: %s", fileNames);
		file.close();
		return;
	}

	std::string vertSource((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	// Fragment
	file.open(path + fileNames + ".frag");

	if(!file.is_open()) {
		printf("Failed to load Fragment Shader: %s", fileNames);
		file.close();
		return;
	}

	std::string fragSource((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	Shader shader = Shader();
	if(shader.LoadShader(vertSource, fragSource)) {
		shaders.insert(std::pair<const char*, Shader>(fileNames, shader));
	}
}
