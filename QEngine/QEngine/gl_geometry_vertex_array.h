#pragma once
#include "gl_base_vertex_array.h"
#include "gl_geometry.h"
#include "gl_vertex.h"
class GLGeometryVertexArray :public GLBaseVertexArray {
private:
	GLArrayBuffer m_vbo;
	GLArrayBuffer m_ebo;
	GLuint indexCount;
public:
	GLGeometryVertexArray(GLenum usage = GL_STATIC_DRAW);

	void FillGeometryData(GeometryPointer geometry);

	void DrawElement();
};