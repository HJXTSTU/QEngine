#include "gl_material.h"

MaterialPointer Material::AsSharedPtr() {
	return shared_from_this();
}

void Material::Use(const glm::mat4 &model) {
	m_shader.use();
	m_shader.setMat4("model", model);
	m_shader.setMat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
}

void Material::Use(const glm::mat4 &model, const RenderTexture &lightBuffer) {
	m_shader.use();
	m_shader.setMat4("model", model);
	m_shader.setMat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
}

void Material::Use(const glm::mat4 &model, Shader &shader) {
	shader.setMat4("model", model);
	shader.setMat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
}



Material::~Material() {
	glDeleteShader(m_shader.ID);
}