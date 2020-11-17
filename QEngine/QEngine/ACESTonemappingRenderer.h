#pragma once
#include "constant.h"
#include "PostprocessRenderer.h"
#include "gl_util.h"
#include "util.h"
class ACESTonemappingRenderer :public PostprocessRenderer {
private:
	float m_fAdaptedIlumination;
	RenderTexture m_rtRenderTexture;

public:
	
	ACESTonemappingRenderer();
	
	void setAdaptedIlumination(float value) { m_fAdaptedIlumination = value; }

	const RenderTexture& Render(const RenderTexture &image);
};