#include "World.h"

void World::InitWorld() {
	// Create Chunks
	for(int y = 0; y < WORLD_SIZE; y++) {
		for(int z = 0; z < WORLD_SIZE; z++) {
			for(int x = 0; x < WORLD_SIZE; x++) {
				chunks[x][y][z] = new Chunk(this, glm::vec3(
					Chunk::size * (x - (WORLD_SIZE / 2)),
					Chunk::size * y,
					Chunk::size * (z - (WORLD_SIZE / 2))),
					glm::ivec3(x, y, z)
				);
			}
		}
	}

	voxelShader = ShaderManager::GetShader("voxel");
	shadow = new ShadowMapping(1024 * 2);
	totalVoxels = 0;

	// Set Texture Unit
	voxelShader->SetTextureUnit("shadowMap[0]", 0);
	voxelShader->SetTextureUnit("shadowMap[1]", 1);
	voxelShader->SetTextureUnit("shadowMap[2]", 2);
	voxelShader->SetTextureUnit("shadowMap[3]", 3);
}

void World::DestroyWorld() {
	// DESTROY EVERYTHING
	for(int y = 0; y < WORLD_SIZE; y++) {
		for(int z = 0; z < WORLD_SIZE; z++) {
			for(int x = 0; x < WORLD_SIZE; x++) {
				chunks[x][y][z]->ClearChunk();
			}
		}
	}
}

void World::AddBlocks(const std::vector<Voxel>& voxels) {
	for(int i = 0; i < voxels.size(); i++) {
		glm::ivec3 chunkIndex = GetChunkIndex(voxels[i].position);
		
		// Out of bounds
		if(chunkIndex.x < 0 || chunkIndex.x >= WORLD_SIZE || chunkIndex.y < 0 || chunkIndex.y >= WORLD_SIZE || chunkIndex.z < 0 || chunkIndex.z >= WORLD_SIZE) {
			continue;
		}

		Chunk* chunk = chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z];
		Voxel* vox = new Voxel(voxels[i]);

		// Check if voxel is a border
		// LEFT
		if((int)vox->position.x == chunk->Position().x - (int)Chunk::size / 2) { 
			if(chunkIndex.x - 1 >= 0) {
				chunks[chunkIndex.x - 1][chunkIndex.y][chunkIndex.z]->SetDirty(true);
			}
		}
		// RIGHT
		if((int)vox->position.x == chunk->Position().x + (int)Chunk::size / 2) {
			if(chunkIndex.x + 1 < WORLD_SIZE) {
				chunks[chunkIndex.x + 1][chunkIndex.y][chunkIndex.z]->SetDirty(true);
			}
		}
		// FORWARD (positive Z)
		if((int)vox->position.z == chunk->Position().z + (int)Chunk::size / 2) {
			if(chunkIndex.z + 1 < WORLD_SIZE) {
				chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z + 1]->SetDirty(true);
			}
		}
		// BACKWARDS (negative Z)
		if((int)vox->position.z == chunk->Position().z - (int)Chunk::size / 2) {
			if(chunkIndex.z - 1 >= 0) {
				chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z - 1]->SetDirty(true);
			}
		}
		// UP
		if((int)vox->position.y == chunk->Position().y + (int)Chunk::size / 2) {
			if(chunkIndex.y + 1 < WORLD_SIZE) {
				chunks[chunkIndex.x][chunkIndex.y + 1][chunkIndex.z]->SetDirty(true);
			}
		}
		// DOWN
		if((int)vox->position.y == chunk->Position().y - (int)Chunk::size / 2) {
			if(chunkIndex.y - 1 >= 0) {
				chunks[chunkIndex.x][chunkIndex.y - 1][chunkIndex.z]->SetDirty(true);
			}
		}

		if(chunk->AddVoxel(vox))
			++totalVoxels;
	}

	for(int y = 0; y < WORLD_SIZE; y++) {
		for(int z = 0; z < WORLD_SIZE; z++) {
			for(int x = 0; x < WORLD_SIZE; x++) {
				if(chunks[x][y][z]->IsDirty()) {
					chunks[x][y][z]->RegenerateMesh();
					chunks[x][y][z]->SetDirty(false);
				}
			}
		}
	}
}

void World::RemoveBlocks(const std::vector<Voxel>& voxels) {
	for(int i = 0; i < voxels.size(); i++) {
		glm::ivec3 chunkIndex = GetChunkIndex(voxels[i].position);

		// Out of bounds
		if(chunkIndex.x < 0 || chunkIndex.x >= WORLD_SIZE || chunkIndex.y < 0 || chunkIndex.y >= WORLD_SIZE || chunkIndex.z < 0 || chunkIndex.z >= WORLD_SIZE) {
			continue;
		}

		Chunk* chunk = chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z];
		Voxel* voxel = chunk->RemoveVoxel(voxels[i].position);

		if(voxel != nullptr) {
			// Check if voxel is a border
			// LEFT
			if((int)voxels[i].position.x == chunk->Position().x - (int)Chunk::size / 2) {
				if(chunkIndex.x - 1 >= 0) {
					chunks[chunkIndex.x - 1][chunkIndex.y][chunkIndex.z]->SetDirty(true);
				}
			}
			// RIGHT
			if((int)voxels[i].position.x == chunk->Position().x + (int)Chunk::size / 2) {
				if(chunkIndex.x + 1 < WORLD_SIZE) {
					chunks[chunkIndex.x + 1][chunkIndex.y][chunkIndex.z]->SetDirty(true);
				}
			}
			// FORWARD (positive Z)
			if((int)voxels[i].position.z == chunk->Position().z + (int)Chunk::size / 2) {
				if(chunkIndex.z + 1 < WORLD_SIZE) {
					chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z + 1]->SetDirty(true);
				}
			}
			// BACKWARDS (negative Z)
			if((int)voxels[i].position.z == chunk->Position().z - (int)Chunk::size / 2) {
				if(chunkIndex.z - 1 >= 0) {
					chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z - 1]->SetDirty(true);
				}
			}
			// UP
			if((int)voxels[i].position.y == chunk->Position().y + (int)Chunk::size / 2) {
				if(chunkIndex.y + 1 < WORLD_SIZE) {
					chunks[chunkIndex.x][chunkIndex.y + 1][chunkIndex.z]->SetDirty(true);
				}
			}
			// DOWN
			if((int)voxels[i].position.y == chunk->Position().y - (int)Chunk::size / 2) {
				if(chunkIndex.y - 1 >= 0) {
					chunks[chunkIndex.x][chunkIndex.y - 1][chunkIndex.z]->SetDirty(true);
				}
			}

			delete voxel;
			chunk->SetDirty(true);
			--totalVoxels;
		}
	}
}

void World::Render() {
	shadow->BindDepthTextures();

	std::string lightMatrix = "_lightData.lightMatrix[_]";
	std::string cascadeSplit = "_lightData.cascadeSplits[_]";
	for(int i = 0; i < shadow->CASCADE_AMOUNT; i++) {
		std::string c = std::to_string(i);

		lightMatrix.replace(lightMatrix.size() - 2, 1, c);
		cascadeSplit.replace(cascadeSplit.size() - 2, 1, c);

		// Set light matrix
		voxelShader->SetMatrix4(lightMatrix.c_str(), shadow->GetShadowProjectionView(i));
		// Set Cascade Split Lengths
		voxelShader->SetFloat(cascadeSplit.c_str(), shadow->GetCascadeSplit(i));
	}

	voxelShader->SetVector4("_lightData.lightDirection", glm::vec4(Settings::sunDirection, (float)Settings::useLighting));

	// Settings
	voxelShader->SetFloat("_showOutline", (float)Settings::useVoxelOutline);

	// Set model data
	voxelShader->SetMatrix4("_view", Camera::ActiveCamera->View());
	voxelShader->SetMatrix4("_projection", Camera::ActiveCamera->Projection());

	for(int x = 0; x < WORLD_SIZE; x++) {
		for(int y = 0; y < WORLD_SIZE; y++) {
			for(int z = 0; z < WORLD_SIZE; z++) {
				Chunk* chunk = chunks[x][y][z];
				if(chunk != nullptr) {
					if(chunk->IsDirty()) {
						chunk->RegenerateMesh();
					}
					
					chunks[x][y][z]->DrawMesh(*voxelShader);
				}
			}
		}
	}
}

void World::RenderDepthMap() {
	for(int i = 0; i < shadow->CASCADE_AMOUNT; i++) {
		shadow->FrameBuffer_Start(i);
		for(int x = 0; x < WORLD_SIZE; x++) {
			for(int y = 0; y < WORLD_SIZE; y++) {
				for(int z = 0; z < WORLD_SIZE; z++) {
					chunks[x][y][z]->DrawMesh(*shadow->GetDepthShader());
				}
			}
		}
		shadow->ClearBuffer();
	}
	shadow->FrameBuffer_End();
}

Chunk* const World::GetVoxelChunk(const glm::ivec3& voxelPos) {
	if(voxelPos.y < 0) return nullptr;
	glm::ivec3 chunkIndex = glm::ivec3(floor(((voxelPos.x + floor(Chunk::size / 2)) / Chunk::size) + WORLD_SIZE / 2),
									   floor(voxelPos.y / Chunk::size),
									   floor(((voxelPos.z + floor(Chunk::size / 2)) / Chunk::size) + WORLD_SIZE / 2));

	if(chunkIndex.x < 0 || chunkIndex.x >= WORLD_SIZE || chunkIndex.y < 0 || chunkIndex.y >= WORLD_SIZE || chunkIndex.z < 0 || chunkIndex.z >= WORLD_SIZE)
		return nullptr;

	return chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z];
}

Chunk* const World::GetChunk(const glm::ivec3 chunkIndex) {
	if(chunkIndex.x <= 0 || chunkIndex.x >= WORLD_SIZE || chunkIndex.y <= 0 || chunkIndex.y >= WORLD_SIZE || chunkIndex.z <= 0 || chunkIndex.z >= WORLD_SIZE)
		return nullptr;
	return chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z];
}

Voxel* World::GetVoxel(const glm::ivec3& position) {
	Chunk* const chunk = GetVoxelChunk(position);
	if(!chunk) return nullptr;

	return chunk->GetVoxel(position);;
}

bool World::HasNeighbour(const Voxel& voxel, Chunk::Direction direction) {
	Chunk* const chunk = GetVoxelChunk(voxel.position);
	if(!chunk) return false;

	return chunk->HasNeighbour(voxel.position, direction);
}

Voxel* World::GetAndRemoveVoxel(const glm::ivec3& position) {
	Chunk* const chunk = GetVoxelChunk(position);
	if(!chunk) return nullptr;

	glm::ivec3 chunkIndex = chunk->Index();

	// Check if voxel is a border
	// LEFT
	if(position.x == chunk->Position().x - (int)Chunk::size / 2) {
		if(chunkIndex.x - 1 >= 0) {
			chunks[chunkIndex.x - 1][chunkIndex.y][chunkIndex.z]->SetDirty(true);
		}
	}
	// RIGHT
	if(position.x == chunk->Position().x + (int)Chunk::size / 2) {
		if(chunkIndex.x + 1 < WORLD_SIZE) {
			chunks[chunkIndex.x + 1][chunkIndex.y][chunkIndex.z]->SetDirty(true);
		}
	}
	// FORWARD (positive Z)
	if(position.z == chunk->Position().z + (int)Chunk::size / 2) {
		if(chunkIndex.z + 1 < WORLD_SIZE) {
			chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z + 1]->SetDirty(true);
		}
	}
	// BACKWARDS (negative Z)
	if(position.z == chunk->Position().z - (int)Chunk::size / 2) {
		if(chunkIndex.z - 1 >= 0) {
			chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z - 1]->SetDirty(true);
		}
	}
	// UP
	if(position.y == chunk->Position().y + (int)Chunk::size / 2) {
		if(chunkIndex.y + 1 < WORLD_SIZE) {
			chunks[chunkIndex.x][chunkIndex.y + 1][chunkIndex.z]->SetDirty(true);
		}
	}
	// DOWN
	if(position.y == chunk->Position().y - (int)Chunk::size / 2) {
		if(chunkIndex.y - 1 >= 0) {
			chunks[chunkIndex.x][chunkIndex.y - 1][chunkIndex.z]->SetDirty(true);
		}
	}

	Voxel* voxel = chunk->RemoveVoxel(position);
	if(voxel)
		--totalVoxels;

	return voxel;
}

int World::TotalVoxels() {
	return totalVoxels;
}

void World::GetAllVoxels(std::vector<Voxel*>& voxels) {
	for(int y = 0; y < WORLD_SIZE; y++) {
		for(int z = 0; z < WORLD_SIZE; z++) {
			for(int x = 0; x < WORLD_SIZE; x++) {
				chunks[x][y][z]->GetAllVoxelsInChunk(voxels);
			}
		}
	}
}

glm::ivec3 World::GetChunkIndex(glm::vec3 voxelPos) {
	return glm::ivec3(floor(((voxelPos.x + floor(Chunk::size / 2)) / Chunk::size) + WORLD_SIZE / 2),
					  floor((voxelPos.y / Chunk::size)),
					  floor(((voxelPos.z + floor(Chunk::size / 2)) / Chunk::size) + WORLD_SIZE / 2));
}
