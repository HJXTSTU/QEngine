#pragma once
#include "gl_framebuffer.h"
#include "gl_render_texture.h"
#include "constant.h"
#include "gl_object3D.h"
#include "light.h"
#include "gl_camera.h"
#include "direction_light.h"
#include "shader.h"
#include "gl_screen_quad_vertex_array.h"
#include "gl_bound_box.h"
#include "input.h"
using namespace glm;

class ShadowmapRenderer {
private:
	Framebuffer m_framebuffer;
	RenderTexture m_rtShadowmap;
	Shader m_mergeShadowmapShader;
	ScreenQuad m_screenQuad;

public:

	ShadowmapRenderer();

	const RenderTexture &GetShadowmap();

	void Clear();

	void MergeShadowmap(const RenderTexture &otherShadowmap, int lightCount);
};

