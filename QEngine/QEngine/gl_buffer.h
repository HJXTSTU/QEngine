#pragma once
#include "gl_element.h"
#include <glm/matrix.hpp>

using namespace glm;
template<GLuint uiBufferTarget = GL_ARRAY_BUFFER, GLuint uiUsage = GL_STATIC_DRAW>
class GLBuffer :public GLElement {
protected:
	GLsizeiptr m_iTotalSize;
	GLsizeiptr m_iOppicupied;
protected:
	void resize(GLsizeiptr size) {
		if (this->m_iTotalSize > 0) {
			GLuint newBuffer;
			glGenBuffers(1, &newBuffer);
			glBindBuffer(uiBufferTarget, newBuffer);
			glBufferData(uiBufferTarget, size, NULL, uiUsage);
			glBindBuffer(uiBufferTarget, 0);
			glBindBuffer(GL_COPY_READ_BUFFER, id);
			glBindBuffer(GL_COPY_WRITE_BUFFER, newBuffer);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_iTotalSize);
			glBindBuffer(GL_COPY_READ_BUFFER, 0);
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
			glDeleteBuffers(1, &this->id);

			this->id = newBuffer;
		}
		else {
			glBindBuffer(uiBufferTarget, id);
			glBufferData(uiBufferTarget, size, NULL, uiUsage);
			glBindBuffer(uiBufferTarget, 0);
		}
		this->m_iTotalSize = size;
	}
public:
	GLBuffer()
		:m_iTotalSize(0),
		m_iOppicupied(0) {
		glGenBuffers(1, &this->id);
	}

	void Allocate(GLsizeiptr size) {
		resize(size);
	}

	void BindBuffer() {
		glBindBuffer(uiBufferTarget, id);
	}

	void UnBindBuffer() {
		glBindBuffer(uiBufferTarget, id);
	}

	void BufferData(GLsizeiptr size, const void* values) {
		glBufferData(uiBufferTarget, size, values, uiUsage);
		m_iOppicupied = size;
		m_iTotalSize = size;
	}

	template<class T>
	void AddValue(T value) {
		if (uiBufferTarget == 0)return;

		if (m_iOppicupied + sizeof(T) >= m_iTotalSize) {
			this->resize(m_iOppicupied + sizeof(T));
		}
		glBindBuffer(uiBufferTarget, id);
		glBufferSubData(uiBufferTarget, m_iOppicupied, sizeof(T), &value);
		glBindBuffer(uiBufferTarget, 0);
		m_iOppicupied += sizeof(T);
	}

	void AddArray(GLsizeiptr size, const void* values) {
		if (uiBufferTarget == 0)return;
		if (m_iOppicupied + size >= m_iTotalSize) {
			this->resize(m_iOppicupied + size);
		}
		glBindBuffer(uiBufferTarget, id);
		glBufferSubData(uiBufferTarget, m_iOppicupied, size, values);
		glBindBuffer(uiBufferTarget, 0);
		m_iOppicupied += size;
	}

	template<class T>
	bool ReplaceValueData(GLsizeiptr startOffset, T value) {
		if (startOffset + sizeof(T) > m_iOppicupied)return false;
		glBindBuffer(uiBufferTarget, id);
		glBufferSubData(uiBufferTarget, startOffset, sizeof(T), &value);
		glBindBuffer(uiBufferTarget, 0);
		return true;
	}

	bool ReplaceArrayData(GLsizeiptr startOffset, GLsizeiptr size, const void* values) {
		if (uiBufferTarget == 0)return false;
		if (startOffset + size > m_iOppicupied)return false;
		glBindBuffer(uiBufferTarget, id);
		glBufferSubData(uiBufferTarget, startOffset, size, values);
		glBindBuffer(uiBufferTarget, 0);
		return true;
	}

	template<class T>
	bool FillValueData(GLsizeiptr startOffset, T value) {
		if (startOffset + sizeof(T) >= m_iTotalSize)return false;
		glBindBuffer(uiBufferTarget, id);
		glBufferSubData(uiBufferTarget, startOffset, sizeof(T), &value);
		glBindBuffer(uiBufferTarget, 0);
		return true;
	}

	bool FillArrayData(GLsizeiptr startOffset, GLsizeiptr size, const void* values) {
		if (uiBufferTarget == 0)return false;
		if (startOffset + size >= m_iTotalSize)return false;
		glBindBuffer(uiBufferTarget, id);
		glBufferSubData(uiBufferTarget, startOffset, size, values);
		glBindBuffer(uiBufferTarget, 0);
		return true;
	}

	GLsizeiptr Size() { return m_iTotalSize; }

	void Clear() {
		glDeleteBuffers(m_iTotalSize, &this->id);
		id = 0;
		glGenBuffers(1, &this->id);
		m_iTotalSize = 0;
		m_iOppicupied = 0;
	}

	~GLBuffer() {
		if (this->id > 0) {
			glDeleteBuffers(m_iTotalSize, &this->id);
			this->id = 0;
		}
	}
public:

	static GLBuffer Create(GLsizeiptr size) {
		GLBuffer res = GLBuffer<uiBufferTarget, uiUsage>();
		res.Allocate(size);
		return res;
	}
};