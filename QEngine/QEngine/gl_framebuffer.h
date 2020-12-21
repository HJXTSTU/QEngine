#pragma once
#include "gl_element.h"
#include "gl_render_texture.h"
#include "gl_cube_map.h"
#include "constant.h"
class Framebuffer:public GLElement {
protected:
	virtual void attachColorAttachment(RenderTexture &color_texture, GLuint num);

	virtual void attachDepthStencilAttachment(RenderTexture &depth_texture);

	virtual void attachDepthAttachment(RenderTexture &depth_texture);

	virtual void attachDepthAttachment(CubeMap &depth_textures);
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

#ifdef ENABLE_MULTISAMPLE
class MultiSampleFramebuffer :public GLElement {
protected:
	virtual void attachColorAttachment(MultiSampleRenderTexture &color_texture, GLuint num);

	virtual void attachDepthStencilAttachment(MultiSampleRenderTexture &depth_texture);

	virtual void attachDepthAttachment(MultiSampleRenderTexture &depth_texture);
public:
	MultiSampleFramebuffer();

	void UseFramebuffer();

	void UnUseFramebuffer();

	bool CheckFramebufferStatus();

	void AttachColorAttachment(MultiSampleRenderTexture &color_texture, GLuint num);

	void AttachDepthStencilAttachment(MultiSampleRenderTexture &depth_texture);

	void AttachDepthAttachment(MultiSampleRenderTexture &depth_texture);

	void CompleteSetupFramebuffer(GLuint attachment_cnt);

	void CopyToIntermediateFramebuffer(Framebuffer &intermediateFramebuffer, GLuint width,GLuint height, GLbitfield mask,GLenum filter_mode) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->GetID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFramebuffer.GetID());
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, filter_mode);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	~MultiSampleFramebuffer();
};
#endif