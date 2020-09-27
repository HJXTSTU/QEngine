#pragma once
#include "gl_element.h"
#include "gl_render_texture.h"
class Framebuffer:public GLElement {
public:
	Framebuffer();

	void UseFramebuffer();

	void UnUseFramebuffer();

	bool CheckFramebufferStatus();

	void AttachColorAttachment(RenderTexture &color_texture, GLuint num);

	void AttachDepthStencilAttachment(RenderTexture &depth_texture);

	~Framebuffer();
};