#pragma once
#include "light.h"
#include "gl_screen_quad_vertex_array.h"
#include "gl_framebuffer.h"
#include "gl_render_texture.h"
#include "constant.h"
#include "gl_util.h"
class LightAccmulatePass {
private:
	Shader m_directionLightShader;
	Shader m_pointLightShader;
	Shader m_spotLightShader;
	
	Framebuffer m_lightAccmulateFramebuffer;
	RenderTexture m_rtLightBuffer;
	ScreenQuad m_screenQuad;
public:
	
	LightAccmulatePass();

	void ClearLightBuffer();
	
	void AccmulateLightShading(LightPointer light,
		const RenderTexture &normalBuffer, const RenderTexture &depthBuffer);

	RenderTexture &GetLightBuffer();
};