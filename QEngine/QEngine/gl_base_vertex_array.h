#pragma once
#include "gl_element.h"
class GLBaseVertexArray :public GLElement {
protected:
	void Use();

	void UnUse();

	void EnableVertexAttrib(GLuint index, GLuint type, GLuint cnt, GLboolean normalized, GLuint stride, GLuint startOffset);
public:
	GLBaseVertexArray();

};