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
	GLuint texId = color_texture.GetID();
	this->UseFramebuffer();
	glBindTexture(GL_TEXTURE_2D, texId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, GL_TEXTURE_2D, texId, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	this->UnUseFramebuffer();
}

void Framebuffer::AttachDepthStencilAttachment(RenderTexture &depth_texture) {
	GLuint texId = depth_texture.GetID();
	this->UseFramebuffer();
	glBindTexture(GL_TEXTURE_2D, texId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	this->UnUseFramebuffer();
}

Framebuffer::~Framebuffer() {
	if (id) {
		glDeleteBuffers(1, &id);
	}
}