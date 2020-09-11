#pragma once
#include "constant.h"
#include "gl_buffer.h"
#include <memory>

template<GLuint uiBindingIndex,GLuint uiBufferSize>
class GLUniformBuffer :protected GLBuffer<GL_UNIFORM_BUFFER> {
public:
	GLUniformBuffer():GLBuffer<GL_UNIFORM_BUFFER>() {
		resize(uiBufferSize);
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBindBufferBase(GL_UNIFORM_BUFFER, uiBindingIndex, id);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	bool RefleshBufferArrayData(GLsizeiptr startOffset, GLsizeiptr size, const void* values) {
		return ReplaceArrayData(startOffset, size, values);
	}

	template<typename T>
	bool RefleshBufferData(GLsizeiptr sizeOffset, T value) {
		return ReplaceValueData<T>(sizeOffset, value);
	}

	bool FillBufferArrayData(GLsizeiptr startOffset, GLsizeiptr size, const void* values) {
		return FillArrayData(startOffset, size, values);
	}

	template<typename T>
	bool FillBufferData(GLsizeiptr sizeOffset, T value) {
		return FillValueData<T>(sizeOffset, value);
	}
};
