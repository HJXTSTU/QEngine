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
public:
	RenderTexture();

	void Initialize(GLuint width, GLuint height, GLuint internalformat, GLenum format, GLenum type);
	
	void SetMinFilter(GLuint filter_mode);

	void SetMagFilter(GLuint filter_mode);

	void SetWrapS(GLuint warp_mode);

	void SetWrapT(GLuint warp_mode);

	void SetWrapR(GLuint warp_mode);

	void SetBorderColor(GLfloat borderColor[4]);

	~RenderTexture();
};