#pragma once
#include "gl_screen_quad_vertex_array.h"
#include "shader.h"
#include "gl_util.h"
class CopyDepthRenderer {
private:
	ScreenQuad m_screenQuad;
	Shader m_copyShader;
public:
	CopyDepthRenderer();

	void ShowDepth(GLuint depthTex);
};