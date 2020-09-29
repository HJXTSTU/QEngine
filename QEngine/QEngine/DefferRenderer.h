#pragma once
#include "gl_renderer.h"
#include "GBuffer.h"
#include "CopyRenderer.h"
#include "constant.h"
#include "input.h"
#include "CopyDepthRenderer.h"
class DefferRenderer :public BaseRenderer {
private:
	GBuffer m_gbuffer;
	CopyRenderer copyRenderer;
	CopyDepthRenderer copyDepthRenderer;
public:
	virtual void OnRender(BaseWorldPointer pWorld) {
		const shared_ptr<Object3D> root = pWorld->Root();
		m_gbuffer.Render(root);

		glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
		copyDepthRenderer.ShowDepth(m_gbuffer.GetDepthTexture().GetID());
	}
};