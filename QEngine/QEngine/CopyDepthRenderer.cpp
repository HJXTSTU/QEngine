#include "CopyDepthRenderer.h"

CopyDepthRenderer::CopyDepthRenderer() {
	m_copyShader.LoadShader("screen_quad_vertex_shader_util.vs", "copy_depth_shader.frag");
	m_copyShader.use();
	m_copyShader.setInt("depthTex", 0);
}

void CopyDepthRenderer::ShowDepth(GLuint depthTex) {
	m_copyShader.use();
	UseTexture(0, depthTex);
	m_screenQuad.DrawScreenQuad();
}