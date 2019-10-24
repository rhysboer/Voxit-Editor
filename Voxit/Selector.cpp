#include "Selector.h"

Selector::Selector() {

}

Selector::~Selector() {
}

void Selector::Init() {
	float vertices[] = {
		// TOP VERTICES				TEX COORDS			DOMINATE AXIS
		-0.5f,  0.5f,  0.5f,/*FL*/	1.0f, 0.0f,			0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,/*BL*/	1.0f, 1.0f,			0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,/*BR*/	0.0f, 1.0f,			0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,/*FR*/	0.0f, 0.0f,			0.0f, 1.0f,
		 // BOT VERTICES								
		-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,			0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		1.0f, 1.0f,			0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,			0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,			0.0f, 1.0f,
		  // LEFT VERTICES								
		-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,			1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		1.0f, 1.0f,			1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,			1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,			1.0f, 1.0f,
		  // RIGHT VERTICES						
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,			1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 1.0f,			1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,		0.0f, 1.0f,			1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,		0.0f, 0.0f,			1.0f, 1.0f,
		   // BACK VERTICES								
		-0.5f, -0.5f, -0.5f,		1.0f, 0.0f,			0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,			0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,		0.0f, 1.0f,			0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,		0.0f, 0.0f,			0.0f, 0.0f,
			// FRONT VERTICES							
		-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,			0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,		1.0f, 1.0f,			0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,		0.0f, 1.0f,			0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,			0.0f, 0.0f
	};

	unsigned int indicies[] = {
		// TOP
		0, 1, 3,
		1, 2, 3,
		// BOT
		4, 5, 7,
		5, 6, 7,
		// LEFT
		8, 9, 11,
		9, 10, 11,
		// RIGHT
		12, 13, 15,
		13, 14, 15,
		// BACK
		16, 17, 19,
		17, 18, 19,
		// FRONT
		20, 21, 23,
		21, 22, 23
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 168 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	SetShader(ShaderManager::GetShader("selector"));

}

void Selector::SetStart(glm::vec3 pos) {
	this->start = pos;
	shader->SetVector3("_start", pos);
}

void Selector::SetEnd(glm::vec3 pos) {
	this->end = pos;
	shader->SetVector3("_end", pos);
}

glm::vec3 Selector::GetStart() {
	return start;
}

glm::vec3 Selector::GetEnd() {
	return end;
}

glm::vec3 Selector::GetDistance() {
	return glm::abs(start - end) + glm::vec3(1.0f);
}

void Selector::Clear() {
	start = glm::vec3(0.0f);
	end = glm::vec3(0.0f);
}

void Selector::OnDraw(Camera* camera) {
	Object::OnDraw(camera);
	if(!isActive) return;

	glDepthMask(GL_FALSE);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);
}
