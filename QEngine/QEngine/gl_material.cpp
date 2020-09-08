#include "gl_material.h"

MaterialPointer Material::AsSharedPtr() {
	return shared_from_this();
}


Material::~Material() {
	glDeleteShader(m_shader.ID);
}