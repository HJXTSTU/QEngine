#pragma once
#include "gl_element.h"
#include <glm/matrix.hpp>

class GLArrayBuffer : public GLElement {
private:
	GLsizeiptr m_totalSize;
	GLsizeiptr m_oppicupied;
	GLenum m_usage;
	GLuint m_currentBufferTarget;
private:
	void resize(GLsizeiptr size);
public:
	GLArrayBuffer(GLenum usage = GL_STATIC_DRAW);

	void BindTo(GLuint buffer_target);

	void UnBind();

	void Allocate(GLsizeiptr size);

	template<class T>
	void AddValue(T value) {
		if (m_currentBufferTarget == 0)return;
		if (m_oppicupied + sizeof(T) >= m_totalSize) {
			this->resize(m_oppicupied + sizeof(T));
		}
		glBufferSubData(m_currentBufferTarget, m_oppicupied, sizeof(T), &value);
		m_oppicupied += sizeof(T);
	}

	void AddArray(GLsizeiptr size, const void* values);

	template<class T>
	bool ReplaceValueData(GLsizeiptr startOffset, T value) {
		if (startOffset + sizeof(T) > this->m_oppicupied)return false;
		glBufferSubData(m_currentBufferTarget, startOffset, sizeof(T), &value);
		return true;
	}

	bool ReplaceArrayData(GLsizeiptr startOffset, GLsizeiptr size, const void* values);


	void Clear();
public:
	static GLArrayBuffer Create(GLenum usage);

	static GLArrayBuffer Create(GLsizeiptr size, GLenum usage);
};
