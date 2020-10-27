#pragma once
#include "gl_screen_quad_vertex_array.h"
#include "shader.h"
#include "gl_util.h"
class CopyRenderer {
private:
	ScreenQuad m_screenQuad;
	Shader m_copyShader;
public:
	CopyRenderer();

	void CopyToScreen(GLuint tex);
};