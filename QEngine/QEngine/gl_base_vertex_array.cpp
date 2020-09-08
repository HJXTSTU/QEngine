#include "gl_base_vertex_array.h"
GLBaseVertexArray::GLBaseVertexArray() {
	glGenVertexArrays(1, &this->id);
}

void GLBaseVertexArray::Use() {
	glBindVertexArray(this->id);
}

void GLBaseVertexArray::UnUse() {
	glBindVertexArray(0);
}

void GLBaseVertexArray::EnableVertexAttrib(GLuint index, GLuint type, GLuint cnt, GLboolean normalized, GLuint stride, GLuint startOffset) {
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, cnt, type, normalized, stride, (void*)startOffset);
}