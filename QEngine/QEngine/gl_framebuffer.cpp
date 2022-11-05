#include "gl_framebuffer.h"
Framebuffer::Framebuffer() {
	glGenFramebuffers(1, &id);
}

void Framebuffer::UseFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void Framebuffer::UnUseFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Framebuffer::CheckFramebufferStatus() {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::AttachColorAttachment(RenderTexture &color_texture, GLuint num) {
	this->UseFramebuffer();
	this->attachColorAttachment(color_texture, num);
	this->UnUseFramebuffer();
}

void Framebuffer::AttachDepthStencilAttachment(RenderTexture &depth_texture) {
	this->UseFramebuffer();
	this->attachDepthStencilAttachment(depth_texture);
	this->UnUseFramebuffer();
}

void  Framebuffer::AttachDepthAttachment(RenderTexture &depth_texture) {
	this->UseFramebuffer();
	this->attachDepthAttachment(depth_texture);
	this->UnUseFramebuffer();
}

void  Framebuffer::AttachDepthAttachment(CubeMap &depth_textures) {
	this->UseFramebuffer();
	this->attachDepthAttachment(depth_textures);
	this->UnUseFramebuffer();
}

void Framebuffer::attachColorAttachment(RenderTexture &color_texture, GLuint num) {
	GLuint texId = color_texture.GetID();
	glBindTexture(GL_TEXTURE_2D, texId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, GL_TEXTURE_2D, texId, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::attachDepthStencilAttachment(RenderTexture &depth_texture){
	GLuint texId = depth_texture.GetID();
	glBindTexture(GL_TEXTURE_2D, texId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::attachDepthAttachment(RenderTexture &depth_texture) {
	GLuint texId = depth_texture.GetID();
	glBindTexture(GL_TEXTURE_2D, texId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::attachDepthAttachment(CubeMap &depth_textures) {
	GLuint texId = depth_textures.GetID();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void  Framebuffer::CompleteSetupFramebuffer(GLuint attachment_cnt) {
	GLuint *attachments = new GLuint[attachment_cnt];
	for (int i = 0; i < attachment_cnt; i++) {
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	UseFramebuffer();
	glDrawBuffers(attachment_cnt, attachments);
	UnUseFramebuffer();
	delete[] attachments;
}

Framebuffer::~Framebuffer() {
	if (id) {
		glDeleteBuffers(1, &id);
	}
}

#ifdef ENABLE_MULTISAMPLE
MultiSampleFramebuffer::MultiSampleFramebuffer(){
	glGenFramebuffers(1, &id);
}

void MultiSampleFramebuffer::UseFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void MultiSampleFramebuffer::UnUseFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool MultiSampleFramebuffer::CheckFramebufferStatus() {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void MultiSampleFramebuffer::AttachColorAttachment(MultiSampleRenderTexture &color_texture, GLuint num) {
	this->UseFramebuffer();
	this->attachColorAttachment(color_texture, num);
	this->UnUseFramebuffer();
}

void MultiSampleFramebuffer::AttachDepthStencilAttachment(MultiSampleRenderTexture &depth_texture) {
	this->UseFramebuffer();
	this->attachDepthStencilAttachment(depth_texture);
	this->UnUseFramebuffer();
}

void MultiSampleFramebuffer::AttachDepthAttachment(MultiSampleRenderTexture &depth_texture) {
	this->UseFramebuffer();
	this->attachDepthAttachment(depth_texture);
	this->UnUseFramebuffer();
}

void MultiSampleFramebuffer::CompleteSetupFramebuffer(GLuint attachment_cnt) {
	GLuint *attachments = new GLuint[attachment_cnt];
	for (int i = 0; i < attachment_cnt; i++) {
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	UseFramebuffer();
	glDrawBuffers(attachment_cnt, attachments);
	UnUseFramebuffer();
	delete[] attachments;
}

void MultiSampleFramebuffer::attachColorAttachment(MultiSampleRenderTexture &color_texture, GLuint num) {
	GLuint texId = color_texture.GetID();
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, GL_TEXTURE_2D_MULTISAMPLE, texId, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void MultiSampleFramebuffer::attachDepthStencilAttachment(MultiSampleRenderTexture &depth_texture) {
	GLuint texId = depth_texture.GetID();
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, texId, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void MultiSampleFramebuffer::attachDepthAttachment(MultiSampleRenderTexture &depth_texture) {
	GLuint texId = depth_texture.GetID();
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, texId, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

MultiSampleFramebuffer::~MultiSampleFramebuffer() {
	if (id) {
		glDeleteBuffers(1, &id);
	}
}
#endif 
