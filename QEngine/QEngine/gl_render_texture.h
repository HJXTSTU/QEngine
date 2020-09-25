#pragma once
#include "gl_element.h"
#include <memory>
#include <assert.h>

class RenderTexture;
typedef std::shared_ptr<RenderTexture> RenderTexturePointer;

class RenderTexture:public GLElement {
private:
	GLuint m_uWidth;
	GLuint m_uHeight;
	GLint  m_iInternalFormat;
	GLenum m_eFormat;
	GLenum m_eType;
public:
	RenderTexture();

	const RenderTexture &SetInternalFormat(GLint internalFormat) {
		m_iInternalFormat = internalFormat;
		return *this;
	}

	const RenderTexture &SetFormat(GLenum format) {
		m_eFormat = format;
		return *this;
	}

	const RenderTexture &SetType(GLenum type) {
		m_eType = type;
		return *this;
	}

	void Initialize(GLuint width, GLuint height);

	~RenderTexture();
};