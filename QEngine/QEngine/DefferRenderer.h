#pragma once
#include "gl_renderer.h"
#include "GBuffer.h"
#include "CopyRenderer.h"
#include "constant.h"
#include "input.h"
#include "CopyDepthRenderer.h"
#include "LightAccmulatePass.h"
#include "shadow_map_renderer.h"
#include "GaussianBlurRenderer.h"
#include "ACESTonemappingRenderer.h"

class DefferRenderer :public BaseRenderer {
private:
	GBuffer m_gbuffer;
	LightAccmulatePass m_lightAccmulatePass;
	ShadowmapRenderer m_shadowmapRenderer;
	GaussianBlurRenderer m_gaussianBlurRenderer;
	ACESTonemappingRenderer m_acesTonemappingRenderer;

	Framebuffer m_finalFramebuffer;
	RenderTexture m_finalRenderTexture;
	RenderTexture m_finalDepthTexture;

	CopyRenderer copyRenderer;
	CopyDepthRenderer copyDepthRenderer;
public:

	DefferRenderer();

	virtual void OnRender(BaseWorldPointer pWorld);
};