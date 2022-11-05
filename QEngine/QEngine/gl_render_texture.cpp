#include "gl_render_texture.h"
#include <glm/glm.hpp>

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
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderTexture::SetMinFilter(GLuint filter_mode) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_mode);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderTexture::SetMagFilter(GLuint filter_mode) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mode);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderTexture::SetWrapS(GLuint warp_mode) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warp_mode);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderTexture::SetWrapT(GLuint warp_mode) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warp_mode);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderTexture::SetWrapR(GLuint warp_mode) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, warp_mode);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderTexture::SetBorderColor(GLfloat borderColor[4]) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);
}

RenderTexture::~RenderTexture()
{
	glDeleteTextures(1, &id);
}


#ifdef ENABLE_MULTISAMPLE
MultiSampleRenderTexture::MultiSampleRenderTexture()
{
	glGenTextures(1, &id);
}

void  MultiSampleRenderTexture::Initialize(GLuint width, GLuint height, GLuint internalformat)
{
	assert(id != 0);
	m_uWidth = width;
	m_uHeight = height;
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLES_COUNT, internalformat, m_uWidth, m_uHeight, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}


void  MultiSampleRenderTexture::SetMinFilter(GLuint filter_mode) {
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, filter_mode);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void  MultiSampleRenderTexture::SetMagFilter(GLuint filter_mode) {
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, filter_mode);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void  MultiSampleRenderTexture::SetWrapS(GLuint warp_mode) {
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, warp_mode);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void  MultiSampleRenderTexture::SetWrapT(GLuint warp_mode) {
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, warp_mode);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void  MultiSampleRenderTexture::SetWrapR(GLuint warp_mode) {
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_R, warp_mode);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void  MultiSampleRenderTexture::SetBorderColor(GLfloat borderColor[4]) {
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	glTexParameterfv(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

MultiSampleRenderTexture::~MultiSampleRenderTexture() {
	glDeleteTextures(1, &id);
}
#endif
