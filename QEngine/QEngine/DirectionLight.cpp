#include "direction_light.h"

DirectionLight::DirectionLight(glm::vec3 color, glm::vec3 direction)
	:Light(LightType::DIRECTION, color), m_vLightDirection(direction) {

}

void DirectionLight::SetupLightParams(Shader &shader) {
	Light::SetupLightParams(shader);
	shader.setVec3("LightDirection", m_vLightDirection);
}

//void DirectionLight::SetupShadowmapMatrix(Shader &shader, glm::vec3 center) {
//	//	Light::SetupShadowmapMatrix(shader);
//	GLfloat near_plane = 0.1f, far_plane = 200.0f;
//	glm::mat4 lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_plane, far_plane);
//	
//	glm::vec3 up = glm::vec3(0, 1, 0);
//	glm::vec3 front = glm::normalize(m_vLightDirection);
//	glm::vec3 right = glm::normalize(glm::cross(front, up));
//	up = glm::normalize(glm::cross(right, front));
//	glm::mat4 lightView = glm::lookAt(center - front * 100.0f, center, up);
//	
//	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
//	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
//}

LightPointer DirectionLight::Create(glm::vec3 color, glm::vec3 direction) {
	return LightPointer(new DirectionLight(color, direction));
}


