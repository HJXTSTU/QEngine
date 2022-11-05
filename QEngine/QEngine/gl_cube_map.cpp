#include "gl_cube_map.h"

CubeMap::CubeMap() {
	glGenTextures(1, &id);
}

void CubeMap::Initialize(GLuint width, GLuint height, GLuint internalformat, GLenum format, GLenum type) {
	m_uWidth = width;
	m_uHeight = height;

	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat,
			m_uWidth, m_uHeight, 0, format, type, NULL);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::SetMinFilter(GLuint filter_mode) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter_mode);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::SetMagFilter(GLuint filter_mode) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter_mode);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::SetWrapS(GLuint warp_mode) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, warp_mode);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::SetWrapT(GLuint warp_mode) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, warp_mode);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::SetWrapR(GLuint warp_mode) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, warp_mode);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::SetBorderColor(GLfloat borderColor[4]) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap() {
	glDeleteTextures(1, &id);
}