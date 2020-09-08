#include "gl_geometry_vertex_array.h"

GLGeometryVertexArray::GLGeometryVertexArray(GLenum usage) :m_vbo(usage), m_ebo(usage) {
}

void GLGeometryVertexArray::FillGeometryData(GeometryPointer geometry) {
	this->Use();
	this->m_vbo.BindTo(GL_ARRAY_BUFFER);
	this->m_vbo.AddArray(geometry->vertices.size() * sizeof(Vertex), &geometry->vertices[0]);

	this->m_ebo.BindTo(GL_ELEMENT_ARRAY_BUFFER);
	this->m_ebo.AddArray(geometry->indices.size() * sizeof(Vertex), &geometry->indices[0]);

	this->EnableVertexAttrib(0, GL_FLOAT, 3, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Position));
	this->EnableVertexAttrib(1, GL_FLOAT, 3, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal));
	this->EnableVertexAttrib(2, GL_FLOAT, 2, GL_FALSE, sizeof(Vertex), offsetof(Vertex, UV));
	this->EnableVertexAttrib(3, GL_FLOAT, 3, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Tangents));
	this->UnUse();
	this->indexCount = geometry->indices.size();
}

void GLGeometryVertexArray::DrawElement(){
	this->Use();
	glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);
	this->UnUse();
}
