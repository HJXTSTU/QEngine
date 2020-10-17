#pragma once
#include "gl_renderer.h"
#include "shader.h"
#include "gl_render_texture.h"
#include "constant.h"
#include "gl_framebuffer.h"
#include "gl_object3D.h"
class GBuffer{
private:
	RenderTexture m_rtDepthTexture;
	RenderTexture m_rtVertexNormalTexture;
	RenderTexture m_rtNormalTexture;
	Framebuffer m_framebuffer;
	Shader m_gbufferShader;
public:
	GBuffer();

	void Render(const shared_ptr<Object3D> world);

	const RenderTexture &GetDepthTexture() { return m_rtDepthTexture; }
	
	const RenderTexture &GetVertexNormalTexture() { return m_rtVertexNormalTexture; }

	const RenderTexture &GetNormalTexture() { return m_rtNormalTexture; }

};