#pragma once
#include <fstream>
#include <map>
#include "Shader.h"

#define PATH "./data/shaders/"

class ShaderManager {
public:

	static void InitShaders();
	static Shader* GetShader(const char* name);

private:

	static void AddShader(const char* fileNames);
	static std::map<const char*, Shader> shaders;
};

