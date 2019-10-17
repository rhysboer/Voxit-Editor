#include "Chunk.h"
#include "World.h"

Chunk::Chunk(glm::vec3 pos, glm::ivec3 index) {
	worldPosition = pos;
	worldIndex = index;
	Init(pos);
	isDirty = false;
}

Chunk::~Chunk() {
	// DELETE VOXELS
}

void Chunk::Init(glm::vec3 pos) {
	for(int y = 0; y < size; y++)
		for(int z = 0; z < size; z++)
			for(int x = 0; x < size; x++)
				voxels[x][y][z] = nullptr;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
}

void Chunk::DrawMesh(Shader& shader) {
	if(totalIndexes != 0) {	
		shader.SetVector3("_pos", worldPosition);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, totalIndexes, GL_UNSIGNED_INT, 0);
	}
}

void Chunk::RegenerateMesh() {
	std::vector<float> vertices = std::vector<float>();
	std::vector<unsigned int> index = std::vector<unsigned int>();
	int iCount = 0;
	int toAdd = 0;

	for(int y = 0; y < size; y++) {
		for(int z = 0; z < size; z++) {
			for(int x = 0; x < size; x++) {
				if(voxels[x][y][z] == nullptr) continue;

				toAdd = 0;
				glm::vec3 pos = glm::vec3(x - (size / 2), y, z - (size / 2)); //voxels[x][y][z]->position;
				glm::vec3 col = voxels[x][y][z]->colour;

				if(!HasNeighbour(voxels[x][y][z]->position, Direction::UP)) {
					std::vector<float> verts = std::vector<float>({
						-0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z,/*FL*/	1.0f, 0.0f,		 0.0f,  1.0f,  0.0f,		col.r, col.g, col.b,
						-0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z,/*BL*/	1.0f, 1.0f,		 0.0f,  1.0f,  0.0f,		col.r, col.g, col.b,
						 0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z,/*BR*/	0.0f, 1.0f,		 0.0f,  1.0f,  0.0f,		col.r, col.g, col.b,
						 0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z,/*FR*/	0.0f, 0.0f,		 0.0f,  1.0f,  0.0f,		col.r, col.g, col.b
					});
					
					++toAdd;
					vertices.insert(vertices.end(), verts.begin(), verts.end());
				}

				if(!HasNeighbour(voxels[x][y][z]->position, Direction::DOWN)) {
					std::vector<float> verts = std::vector<float>({
						-0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z,		1.0f, 0.0f,		 0.0f, -1.0f,  0.0f,		col.r, col.g, col.b,
						-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,		1.0f, 1.0f,		 0.0f, -1.0f,  0.0f,		col.r, col.g, col.b,
						 0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,		0.0f, 1.0f,		 0.0f, -1.0f,  0.0f,		col.r, col.g, col.b,
						 0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z,		0.0f, 0.0f,		 0.0f, -1.0f,  0.0f,		col.r, col.g, col.b
					});
				
					++toAdd;
					vertices.insert(vertices.end(), verts.begin(), verts.end());
				}
				
				if(!HasNeighbour(voxels[x][y][z]->position, Direction::WEST)) {
					std::vector<float> verts = std::vector<float>({
						-0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z,		1.0f, 0.0f,		-1.0f,  0.0f,  0.0f,		col.r, col.g, col.b,
						-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,		1.0f, 1.0f,		-1.0f,  0.0f,  0.0f,		col.r, col.g, col.b,
						-0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z,		0.0f, 1.0f,		-1.0f,  0.0f,  0.0f,		col.r, col.g, col.b,
						-0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z,		0.0f, 0.0f,		-1.0f,  0.0f,  0.0f,		col.r, col.g, col.b
					});
				
					++toAdd;
					vertices.insert(vertices.end(), verts.begin(), verts.end());
				}
				
				if(!HasNeighbour(voxels[x][y][z]->position, Direction::EAST)) {
					std::vector<float> verts = std::vector<float>({
						0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z,			1.0f, 0.0f,		 1.0f,  0.0f,  0.0f,		col.r, col.g, col.b,
						0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,			1.0f, 1.0f,		 1.0f,  0.0f,  0.0f,		col.r, col.g, col.b,
						0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z,			0.0f, 1.0f,		 1.0f,  0.0f,  0.0f,		col.r, col.g, col.b,
						0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z,			0.0f, 0.0f,		 1.0f,  0.0f,  0.0f,		col.r, col.g, col.b
					});
				
					++toAdd;
					vertices.insert(vertices.end(), verts.begin(), verts.end());
				}
				
				if(!HasNeighbour(voxels[x][y][z]->position, Direction::NORTH)) {
					std::vector<float> verts = std::vector<float>({
						-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,		1.0f, 0.0f,		 0.0f,  0.0f, -1.0f, 		col.r, col.g, col.b,
						-0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z,		1.0f, 1.0f,		 0.0f,  0.0f, -1.0f, 		col.r, col.g, col.b,
						 0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z,		0.0f, 1.0f,		 0.0f,  0.0f, -1.0f, 		col.r, col.g, col.b,
						 0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,		0.0f, 0.0f,		 0.0f,  0.0f, -1.0f, 		col.r, col.g, col.b
					});
				
					++toAdd;
					vertices.insert(vertices.end(), verts.begin(), verts.end());
				}
				
				if(!HasNeighbour(voxels[x][y][z]->position, Direction::SOUTH)) {
					std::vector<float> verts = std::vector<float>({
						-0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z,		1.0f, 0.0f,		 0.0f,  0.0f,  1.0f,		col.r, col.g, col.b,
						-0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z,		1.0f, 1.0f,		 0.0f,  0.0f,  1.0f,		col.r, col.g, col.b,
						 0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z,		0.0f, 1.0f,		 0.0f,  0.0f,  1.0f,		col.r, col.g, col.b,
						 0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z,		0.0f, 0.0f,		 0.0f,  0.0f,  1.0f,		col.r, col.g, col.b
					});
				
					++toAdd;
					vertices.insert(vertices.end(), verts.begin(), verts.end());
				}

				for(int i = 0; i < toAdd; i++) {
					index.push_back(iCount);
					index.push_back(iCount + 1);
					index.push_back(iCount + 3);
				
					index.push_back(iCount + 1);
					index.push_back(iCount + 2);
					index.push_back(iCount + 3);
				
					iCount += 4;
				}
			}
		}
	}

	SetDirty(false);
	totalIndexes = index.size();
	if(vertices.size() <= 0 || index.size() <= 0) return;

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), &index[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	
	glBindVertexArray(0);
}

bool Chunk::AddVoxel(Voxel* voxel) {
	glm::ivec3 index = glm::ivec3(Mod((int)(voxel->position.x + floor(Chunk::size / 2)), Chunk::size),
								  Mod((int)(voxel->position.y), Chunk::size),
								  Mod((int)(voxel->position.z + floor(Chunk::size / 2)), Chunk::size));
	if(index.x < 0 || index.x > size || index.y < 0 || index.y > size || index.z < 0 || index.z > size) {
		delete voxel;
		return false;
	}

	if(voxels[index.x][index.y][index.z] != nullptr) {
		delete voxel;
		return false;
	}

	this->SetDirty(true);

	voxels[index.x][index.y][index.z] = voxel;
	return true;
}

Voxel* Chunk::RemoveVoxel(const glm::vec3& pos) {
	glm::ivec3 index = WorldPosToIndex(pos);
	if(index.x < 0 || index.x >= size || index.y < 0 || index.y >= size || index.z < 0 || index.z >= size)
		return nullptr;

	Voxel* voxel = voxels[index.x][index.y][index.z];
	voxels[index.x][index.y][index.z] = nullptr;

	SetDirty(true);

	return voxel;
}

Voxel* Chunk::GetVoxel(glm::vec3 position) const {
	glm::ivec3 index = WorldPosToIndex(position);
	if(index.x < 0 || index.x >= size || index.y < 0 || index.y >= size|| index.z < 0 || index.z >= size)
		return nullptr;
	return voxels[index.x][index.y][index.z];
}

void Chunk::GetVoxels(std::vector<Voxel*>& voxels) const {
	for(int y = 0; y < size; y++) {
		for(int z = 0; z < size; z++) {
			for(int x = 0; x < size; x++) {
				Voxel* vox = this->voxels[x][y][z];
				if(vox) {
					voxels.push_back(vox);
				}
			}
		}
	}
}

glm::ivec3 Chunk::DirectionToVec(const Chunk::Direction& dire) const {
	switch(dire) {
		case Chunk::Direction::NORTH:	return glm::vec3( 0.0f, 0.0f,-1.0f);
		case Chunk::Direction::EAST:	return glm::vec3( 1.0f, 0.0f, 0.0f);
		case Chunk::Direction::SOUTH:	return glm::vec3( 0.0f, 0.0f, 1.0f);
		case Chunk::Direction::WEST:	return glm::vec3(-1.0f, 0.0f, 0.0f);
		case Chunk::Direction::UP:		return glm::vec3( 0.0f, 1.0f, 0.0f);
		case Chunk::Direction::DOWN:	return glm::vec3( 0.0f,-1.0f, 0.0f);
		default: return glm::vec3(0.0f);
	}
}

bool Chunk::HasNeighbour(glm::vec3 pos, Chunk::Direction direction) const {
	glm::vec3 dire = DirectionToVec(direction);
	Chunk* const chunk = World::GetVoxelChunk(pos + dire);
	if(!chunk) return false;

	if(chunk->GetVoxel(pos + dire))
		return true;
	return false;
}

glm::vec3 Chunk::WorldPosToIndex(glm::vec3 pos) const {
	return glm::vec3(Mod((int)(pos.x + floor(Chunk::size / 2)), Chunk::size),
					 Mod((int)(pos.y), Chunk::size),
					 Mod((int)(pos.z + floor(Chunk::size / 2)), Chunk::size));
}

glm::vec3 Chunk::Position() {
	return worldPosition;
}

glm::ivec3 Chunk::Index() {
	return worldIndex;
}

void Chunk::SetDirty(bool dirty) {
	isDirty = dirty;
}

bool Chunk::IsDirty() const {
	return isDirty;
}

int Mod(int value, int mod) {
	return ((value %= mod) < 0) ? value + mod : value;
}
