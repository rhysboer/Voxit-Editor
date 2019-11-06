#pragma once
#include <vector>
#include "Settings.h"
#include "ShaderManager.h"
#include "Chunk.h"
#include "Camera.h"

#include "ShadowMapping.h"

class World {
public:
	World() {};
	~World() {};

	void InitWorld();
	void DestroyWorld();

	void AddBlocks(const std::vector<Voxel>& voxels);
	void RemoveBlocks(const std::vector<Voxel>& voxels);

	void Render();
	void RenderDepthMap();

	// Return voxels chunk
	Chunk* const GetVoxelChunk(const glm::ivec3& voxelPos); // Returns chunk the voxel position is in, nullptr if out of bounds
	Chunk* const GetChunk(const glm::ivec3 chunkIndex);	// Returns chunk from chunk index, nullptr if out of bounds 

	// Gets the voxel at the closest position, returns nullptr if no voxel exists or out of bounds
	Voxel* GetVoxel(const glm::ivec3& position);
	bool HasNeighbour(const Voxel& voxel, Chunk::Direction direction);

	// Removes voxel from world, returning the pointer to it. MUST BE DELETED
	Voxel* GetAndRemoveVoxel(const glm::ivec3& position);

	// Returns total voxels
	int TotalVoxels();

	void GetAllVoxels(std::vector<Voxel*>& voxels);

	const static int WORLD_SIZE = 5;
private:

	glm::ivec3 GetChunkIndex(glm::vec3 voxelPos);

	// World Voxel Shader
	Shader* voxelShader;
	ShadowMapping* shadow;
	unsigned int totalVoxels;

	Chunk* chunks[WORLD_SIZE][WORLD_SIZE][WORLD_SIZE]; 
};
