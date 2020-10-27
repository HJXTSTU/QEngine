#pragma once
#include "shader.h"
#include <memory>
enum LightType {
	DIRECTION,
	POINT_LIGHT,
	SPOT_LIGHT,
};

using namespace std;

class Light;
typedef shared_ptr<Light> LightPointer;

class Light {
protected:
	LightType m_eLightType;
	glm::vec3 m_vLightColor;
public:

	Light(LightType type, glm::vec3 color);

	LightType GetType();

	virtual void SetupLightParams(Shader &shader);
};