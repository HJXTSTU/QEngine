#pragma once
#include "gl_element.h"
#include "gl_render_texture.h"
#include "gl_cube_map.h"
class Framebuffer:public GLElement {
public:
	Framebuffer();

	void UseFramebuffer();

	void UnUseFramebuffer();

	bool CheckFramebufferStatus();

	void AttachColorAttachment(RenderTexture &color_texture, GLuint num);

	void AttachDepthStencilAttachment(RenderTexture &depth_texture);

	void AttachDepthAttachment(RenderTexture &depth_texture);

	void AttachDepthAttachment(CubeMap &depth_textures);

	void CompleteSetupFramebuffer(GLuint attachment_cnt);

	~Framebuffer();
};