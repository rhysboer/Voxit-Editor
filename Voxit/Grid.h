#pragma once
#include "Object.h"
#include <vector>

class Camera;

class Grid : public Object {
public:
	Grid();
	Grid(int size);
	~Grid();

	void Init(int size);
	void OnDraw(Camera * const camera) override;

	bool enabled;
private:

	void Add(float x, float y, float z, float r, float g, float b);

	unsigned int axisVAO;
	unsigned int axisVBO;

	std::vector<float> points = std::vector<float>();
};

