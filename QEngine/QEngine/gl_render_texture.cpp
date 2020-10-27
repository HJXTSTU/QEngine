#include "gl_render_texture.h"

RenderTexture::RenderTexture()
{
	glGenTextures(1, &id);
}

void RenderTexture::Initialize(GLuint width, GLuint height,GLuint internalformat,GLenum format,GLenum type)
{
	assert(id != 0);
	m_uWidth = width;
	m_uHeight = height;
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, m_uWidth, m_uHeight, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);
}

RenderTexture::~RenderTexture()
{
	glDeleteTextures(1, &id);
}
