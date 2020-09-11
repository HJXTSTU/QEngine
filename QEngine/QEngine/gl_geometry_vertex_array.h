#pragma once
#include "gl_base_vertex_array.h"
#include "gl_geometry.h"
#include "gl_vertex.h"
#include "gl_buffer.h"
class GLGeometryVertexArray :public GLBaseVertexArray {
private:
	GLBuffer<GL_ARRAY_BUFFER, GL_STATIC_DRAW> m_vbo;
	GLBuffer<GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW> m_ebo;
	GLuint vbo;
	GLuint ebo;
	GLuint indexCount;
public:
	GLGeometryVertexArray();

	void FillGeometryData(GeometryPointer geometry);

	void DrawElement();
};