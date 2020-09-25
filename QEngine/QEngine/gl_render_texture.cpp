#include "gl_render_texture.h"

RenderTexture::RenderTexture()
{
	glGenTextures(1, &id);
}

void RenderTexture::Initialize(GLuint width, GLuint height)
{
	m_uWidth = width;
	m_uHeight = height;
	assert(m_iInternalFormat != 0);
	assert(m_eFormat != 0);
	assert(m_eType != 0);
	assert(id != 0);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, m_iInternalFormat, m_uWidth, m_uHeight, 0, m_eFormat, m_eType, NULL);
}

RenderTexture::~RenderTexture()
{
	glDeleteTextures(1, &id);
}
