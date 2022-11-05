#include "shadow_map_renderer.h"


ShadowmapRenderer::ShadowmapRenderer() {
	m_mergeShadowmapShader.LoadShader("screen_quad_vertex_shader_util.vs", "shadowmap_merge.frag");
	m_mergeShadowmapShader.use();
	m_mergeShadowmapShader.setInt("inputShadowmap", 0);

	m_rtShadowmap.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);

	m_framebuffer.AttachColorAttachment(m_rtShadowmap, 0);
}

const RenderTexture& ShadowmapRenderer::GetShadowmap() { return this->m_rtShadowmap; }

void ShadowmapRenderer::Clear() {
	m_framebuffer.UseFramebuffer();
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	m_framebuffer.UnUseFramebuffer();
}

void ShadowmapRenderer::MergeShadowmap(const RenderTexture &otherShadowmap) {
	m_mergeShadowmapShader.use();
	UseTexture(0, otherShadowmap.GetID());
	m_framebuffer.UseFramebuffer();
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendEquation(GL_ADD);
	m_screenQuad.DrawScreenQuad();
	glDisable(GL_BLEND);
	m_framebuffer.UnUseFramebuffer();
}


