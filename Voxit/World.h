#pragma once
#include <vector>
#include "Settings.h"
#include "ShaderManager.h"
#include "Chunk.h"
#include "Camera.h"

class World {
public:
	static void InitWorld();
	static void DestroyWorld();

	static void AddBlocks(const std::vector<Voxel>& voxels);
	static void RemoveBlocks(const std::vector<Voxel>& voxels);

	static void Draw();
	static void DepthMapDraw(Shader* depthShader);

	// Return voxels chunk
	static Chunk* const GetVoxelChunk(const glm::ivec3& voxelPos); // Returns chunk the voxel position is in, nullptr if out of bounds
	static Chunk* const GetChunk(const glm::ivec3 chunkIndex);	// Returns chunk from chunk index, nullptr if out of bounds 

	// Gets the voxel at the closest position, returns nullptr if no voxel exists or out of bounds
	static Voxel* GetVoxel(const glm::ivec3& position);
	static bool HasNeighbour(const Voxel& voxel, Chunk::Direction direction);

	// Removes voxel from world, returning the pointer to it. MUST BE DELETED
	static Voxel* GetAndRemoveVoxel(const glm::ivec3& position);

	static int TotalBlocks();

	static void GetAllVoxels(std::vector<Voxel*>& voxels);

private:

	static glm::ivec3 GetChunkIndex(glm::vec3 voxelPos);

	static Shader* shader;
	static unsigned int totalVoxels;

	const static int worldSize = 5;
	static Chunk* chunks[worldSize][worldSize][worldSize]; 
};
