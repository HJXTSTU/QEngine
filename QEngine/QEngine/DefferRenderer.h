#pragma once
#include "gl_renderer.h"
#include "GBuffer.h"
#include "CopyRenderer.h"
#include "constant.h"
#include "input.h"
#include "CopyDepthRenderer.h"
#include "LightAccmulatePass.h"
class DefferRenderer :public BaseRenderer {
private:
	GBuffer m_gbuffer;
	LightAccmulatePass m_lightAccmulatePass;
	CopyRenderer copyRenderer;
	CopyDepthRenderer copyDepthRenderer;
public:
	virtual void OnRender(BaseWorldPointer pWorld) {
		const shared_ptr<Object3D> root = pWorld->Root();
		m_gbuffer.Render(root);
		
		m_lightAccmulatePass.ClearLightBuffer();
		vector<LightPointer> lights = pWorld->GetLights();
		for (int i = 0; i < lights.size(); i++) {
			m_lightAccmulatePass.AccmulateLightShading(lights[i],m_gbuffer.GetNormalTexture(),m_gbuffer.GetDepthTexture());
		}
		
		glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		/*copyDepthRenderer.ShowDepth(m_gbuffer.GetDepthTexture().GetID());*/
		//copyRenderer.CopyToScreen(m_lightAccmulatePass.GetLightBuffer().GetID());
		root->OnSurfaceRender(m_lightAccmulatePass.GetLightBuffer());
	}
};