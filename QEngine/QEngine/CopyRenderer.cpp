#include "CopyRenderer.h"
CopyRenderer::CopyRenderer() {
	m_copyShader.LoadShader("screen_quad_vertex_shader_util.vs", "copy_shader.frag");
	m_copyShader.use();
	m_copyShader.setInt("tex", 0);
}

void CopyRenderer::CopyToScreen(GLuint tex) {
	m_copyShader.use();
	UseTexture(0, tex);
	m_screenQuad.DrawScreenQuad();
}