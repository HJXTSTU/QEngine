#pragma once
#include "gl_base_vertex_array.h"
#include "gl_buffer.h"
class ScreenQuad:public GLBaseVertexArray {
private:
	GLBuffer<GL_ARRAY_BUFFER, GL_STATIC_DRAW> m_vbo;
public:
	ScreenQuad();
	
	void DrawScreenQuad();
};