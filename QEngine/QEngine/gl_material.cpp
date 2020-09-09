#include "gl_material.h"

MaterialPointer Material::AsSharedPtr() {
	return shared_from_this();
}

void Material::Use(const glm::mat4 &model) {
	m_shader.use();
	m_shader.setMat4("model", model);
}



Material::~Material() {
	glDeleteShader(m_shader.ID);
}