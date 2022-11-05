#include "PostprocessRenderer.h"

PostprocessRenderer::PostprocessRenderer(const std::string &shader_file) {
	m_shader.LoadShader("screen_quad_vertex_shader_util.vs", shader_file.c_str());
}