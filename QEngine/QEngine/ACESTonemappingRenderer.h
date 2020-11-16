#pragma once
#include "constant.h"
#include "PostprocessRenderer.h"
#include "gl_util.h"
#include "util.h"
class ACESTonemappingRenderer :public PostprocessRenderer {
private:
	float m_fAdaptedIlumination;
	RenderTexture m_rtRenderTexture;

public:
	
	ACESTonemappingRenderer():PostprocessRenderer("aces.frag") {
		m_shader.use();
		m_shader.setInt("image", 0);
		m_rtRenderTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);
		m_framebuffer.AttachColorAttachment(m_rtRenderTexture, 0);
	}
	
	void setAdaptedIlumination(float value) { m_fAdaptedIlumination = value; }

	const RenderTexture& Render(const RenderTexture &image) {
		m_shader.use();
		m_shader.setFloat("AdaptedIlumination", m_fAdaptedIlumination);
		UseTexture(0, image.GetID());

		m_framebuffer.UseFramebuffer();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		m_screen.DrawScreenQuad();
		m_framebuffer.UnUseFramebuffer();
		return m_rtRenderTexture;
	}

};