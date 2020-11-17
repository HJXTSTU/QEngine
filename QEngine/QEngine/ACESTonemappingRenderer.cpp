#include "ACESTonemappingRenderer.h"


ACESTonemappingRenderer::ACESTonemappingRenderer() :PostprocessRenderer("aces.frag") {
	m_shader.use();
	m_shader.setInt("image", 0);
	m_rtRenderTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_framebuffer.AttachColorAttachment(m_rtRenderTexture, 0);
}

const RenderTexture& ACESTonemappingRenderer::Render(const RenderTexture &image) {
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

