#pragma once
#include "gl_element.h"
class Framebuffer:public GLElement {
public:
	Framebuffer() {
		glGenFramebuffers(1, &id);
	}

	void UseFramebuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	}

	void UnUseFramebuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	~Framebuffer() {
		if (id) {
			glDeleteBuffers(1, &id);
		}
	}
};