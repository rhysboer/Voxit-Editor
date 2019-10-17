#pragma once
#include "Object.h"
class Selector : public Object {
public:
	Selector();
	~Selector();

	void Init();

	void SetStart(glm::vec3 pos);
	void SetEnd(glm::vec3 pos);
	glm::vec3 GetStart();
	glm::vec3 GetEnd();
	glm::vec3 GetDistance();

	void Clear();

	void OnDraw(Camera* camera) override;

private:

	glm::vec3 start;
	glm::vec3 end;

	unsigned int ebo;
};

