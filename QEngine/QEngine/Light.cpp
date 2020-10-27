#include "light.h"

Light::Light(LightType type, glm::vec3 color) :m_eLightType(type), m_vLightColor(color) {

}

LightType Light::GetType() { return m_eLightType; }

void Light::SetupLightParams(Shader &shader) {
	shader.setInt("LightType", m_eLightType);
	shader.setVec3("LightColor", m_vLightColor);
}
