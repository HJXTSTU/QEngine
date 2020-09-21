#include "gl_render_texture.h"

RenderTexture::RenderTexture()
{
	glGenTextures(1, &id);
}

RenderTexture::~RenderTexture()
{
	glDeleteTextures(1, &id);
}
