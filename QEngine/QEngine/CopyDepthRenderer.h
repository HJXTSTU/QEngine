#pragma once
#include "gl_screen_quad_vertex_array.h"
#include "shader.h"
#include "gl_util.h"
class CopyDepthRenderer {
private:
	ScreenQuad m_screenQuad;
	Shader m_copyShader;
public:
	CopyDepthRenderer() {
		m_copyShader.LoadShader("screen_quad_vertex_shader_util.vs", "copy_depth_shader.frag");
		m_copyShader.use();
		m_copyShader.setInt("depthTex", 0);
	}

	void ShowDepth(GLuint depthTex) {
		m_copyShader.use();
		UseTexture(0, depthTex);
		m_screenQuad.DrawScreenQuad();
	}
};