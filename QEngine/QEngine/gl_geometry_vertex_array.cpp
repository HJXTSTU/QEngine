#include "gl_geometry_vertex_array.h"

GLGeometryVertexArray::GLGeometryVertexArray() {
}

void GLGeometryVertexArray::FillGeometryData(GeometryPointer geometry) {
	if (this->m_vbo.Size() != 0)
		this->m_vbo.Clear();
	if (this->m_ebo.Size() != 0)
		this->m_ebo.Clear();

	this->Use();

	this->m_vbo.BindBuffer();
	this->m_vbo.BufferData(geometry->vertices.size() * sizeof(Vertex), &geometry->vertices[0]);
	
	this->m_ebo.BindBuffer();
	this->m_ebo.BufferData(geometry->indices.size() * sizeof(GLuint), &geometry->indices[0]);

	this->EnableVertexAttrib(0, GL_FLOAT, 3, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Position));
	this->EnableVertexAttrib(1, GL_FLOAT, 3, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal));
	this->EnableVertexAttrib(2, GL_FLOAT, 2, GL_FALSE, sizeof(Vertex), offsetof(Vertex, UV));
	this->EnableVertexAttrib(3, GL_FLOAT, 3, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Tangents));
	this->UnUse();
	this->m_vbo.UnBindBuffer();
	this->m_ebo.UnBindBuffer();
	this->indexCount = geometry->indices.size();
}

void GLGeometryVertexArray::DrawElement(){
	this->Use();
	glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);
	this->UnUse();
}
