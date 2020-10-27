#pragma once
#include "gl_renderer.h"
#include "GBuffer.h"
#include "CopyRenderer.h"
#include "constant.h"
#include "input.h"
#include "CopyDepthRenderer.h"
#include "LightAccmulatePass.h"
#include "shadow_map_renderer.h"

class DefferRenderer :public BaseRenderer {
private:
	GBuffer m_gbuffer;
	LightAccmulatePass m_lightAccmulatePass;
	ShadowmapRenderer m_shadowmapRenderer;
	CopyRenderer copyRenderer;
	CopyDepthRenderer copyDepthRenderer;
public:
	virtual void OnRender(BaseWorldPointer pWorld);
};