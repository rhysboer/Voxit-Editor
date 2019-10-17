#pragma once
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "Voxel.h"

class World;
class Chunk {
public:
	Chunk(glm::vec3 pos, glm::ivec3 index);
	~Chunk();

	enum class Direction {
		NORTH,
		EAST,
		SOUTH,
		WEST,
		UP,
		DOWN,
	};

	const static int size = 15;

	void Init(glm::vec3 pos);
	void DrawMesh(Shader& shader);
	void RegenerateMesh();

	// Returns false if block is outside of chunk or already exists
	bool AddVoxel(Voxel* voxel);
	// Returns the voxel pointer removed from the chunk, and sets spot nullptr. user must delete pointer
	Voxel* RemoveVoxel(const glm::vec3& pos);

	Voxel* GetVoxel(const glm::vec3 position) const;
	void GetVoxels(std::vector<Voxel*>& voxels) const;

	glm::vec3 Position();
	glm::ivec3 Index();
	
	bool HasNeighbour(glm::vec3 pos, Chunk::Direction direction) const;

	void SetDirty(bool dirty);
	bool IsDirty() const;

private:

	glm::ivec3 DirectionToVec(const Chunk::Direction& dire) const;
	glm::vec3 WorldPosToIndex(glm::vec3 pos) const;

	glm::vec3 worldPosition;
	glm::ivec3 worldIndex;

	Voxel* voxels[size][size][size];

	// If dirty, regenerate mesh
	bool isDirty;

	int totalIndexes = 0;

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};

static int Mod(int value, int mod);
