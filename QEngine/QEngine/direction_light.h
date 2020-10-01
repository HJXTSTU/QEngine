#pragma once
#include "light.h"

class DirectionLight :public Light {
private:
	glm::vec3 m_vLightDirection;
public:
	DirectionLight(glm::vec3 color,glm::vec3 direction)
		:Light(LightType::DIRECTION,color),m_vLightDirection(direction){

	}

	virtual void SetupLightParams(Shader &shader) {
		Light::SetupLightParams(shader);
		shader.setVec3("LightDirection", m_vLightDirection);
	}

	static LightPointer Create(glm::vec3 color, glm::vec3 direction) {
		return LightPointer(new DirectionLight(color, direction));
	}
};