#pragma once
#include "shader.h"
#include "gl_framebuffer.h"
#include "gl_screen_quad_vertex_array.h"
#include "gl_render_texture.h"
#include <string>
class PostprocessRenderer {
protected:
	Framebuffer m_framebuffer;
	Shader m_shader;
	ScreenQuad m_screen;
public:
	PostprocessRenderer(const std::string &shader_file);
};