#include "Grid.h"

void Grid::Add(float x, float y, float z, float r, float g, float b) {
	points.push_back(x);
	points.push_back(y);
	points.push_back(z);

	points.push_back(r);
	points.push_back(g);
	points.push_back(b);
}


Grid::Grid() {
	this->enabled = true;
}

Grid::Grid(int size) {
	this->enabled = true;
	Init(size);
}

Grid::~Grid() {
}

void Grid::Init(int size) {
	for(int i = 0; i < size; i++) {
		Add(-(size / 2.0f) + 0.5f, 0.0f, i + (-(size / 2.0f) + 0.5f),	0.0f, 0.0f, 0.0f);
		Add((size / 2.0f) - 0.5f, 0.0f, i + (-(size / 2.0f) + 0.5f),	0.0f, 0.0f, 0.0f);

		Add(i + (-(size / 2.0f) + 0.5f), 0.0f, -(size / 2.0f) + 0.5f,	0.0f, 0.0f, 0.0f);
		Add(i + (-(size / 2.0f) + 0.5f), 0.0f, (size / 2.0f) - 0.5f,	0.0f, 0.0f, 0.0f);
	}

	// Z Axis
	Add(0.0f, 0.0f, (size / 2.0f) - 0.5f, 0.0f, 0.0f, 0.7f);
	Add(0.0f, 0.0f, (-size / 2.0f) + 0.5f, 0.0f, 0.0f, 0.7f);

	// X Axis
	Add((size / 2.0f) - 0.5f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f);
	Add((-size / 2.0f) + 0.5f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f);

	SetShader(ShaderManager::GetShader("line"));

	this->enabled = true;
	transform.SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));

	// Grid
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Axis
	//float buffer[] = {
	//	0.0f, 0.0f, size / 2.0f,
	//	0.0f, 0.0f, -size / 2.0f,
	//	size / 2.0f, 0.0f, 0.0f,
	//	-size / 2.0f, 0.0f, 0.0f
	//};
	//
	//glGenVertexArrays(1, &axisVAO);
	//glGenBuffers(1, &axisVBO);
	//
	//glBindVertexArray(axisVAO);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	//glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), buffer, GL_STATIC_DRAW);
	//
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Grid::OnDraw(Camera* const camera) {
	if(points.size() <= 0 || !enabled) return;
	Object::OnDraw(camera);

	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, points.size());
}