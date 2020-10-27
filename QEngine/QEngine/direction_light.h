#pragma once
#include "light.h"
class DirectionLight;
typedef shared_ptr<DirectionLight> DirectionLightPointer;

class DirectionLight :public Light {
private:
	glm::vec3 m_vLightDirection;
public:
	DirectionLight(glm::vec3 color, glm::vec3 direction);

	virtual void SetupLightParams(Shader &shader);

	glm::vec3 GetDirection() { return m_vLightDirection; }

	static LightPointer Create(glm::vec3 color, glm::vec3 direction);
};