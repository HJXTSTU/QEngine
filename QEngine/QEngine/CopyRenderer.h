#pragma once
#include "gl_screen_quad_vertex_array.h"
#include "shader.h"
#include "gl_util.h"
class CopyRenderer {
private:
	ScreenQuad m_screenQuad;
	Shader m_copyShader;
public:
	CopyRenderer() {
		m_copyShader.LoadShader("screen_quad_vertex_shader_util.vs", "copy_shader.frag");
		m_copyShader.use();
		m_copyShader.setInt("tex", 0);
	}

	void CopyToScreen(GLuint tex) {
		m_copyShader.use();
		UseTexture(0, tex);
		m_screenQuad.DrawScreenQuad();
	}
};