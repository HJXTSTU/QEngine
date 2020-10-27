#include "DefferRenderer.h"
#include "gl_context.h"

void DefferRenderer::OnRender(BaseWorldPointer pWorld) {
	const shared_ptr<Object3D> root = pWorld->Root();
	m_gbuffer.Render(root);

	m_lightAccmulatePass.ClearLightBuffer();
	vector<LightPointer> lights = pWorld->GetLights();
	for (int i = 0; i < lights.size(); i++) {
		m_lightAccmulatePass.AccmulateLightShading(lights[i], m_gbuffer.GetNormalTexture(), m_gbuffer.GetDepthTexture());
	}

	m_shadowmapRenderer.Clear();
	for (int i = 0; i < lights.size(); i++) {
		m_shadowmapRenderer.OnRender(root, lights[i], Context::mainCamera, m_gbuffer.GetDepthTexture(),m_gbuffer.GetVertexNormalTexture());
	}

	static bool showShadowdepthmap = true;
	if (Input::getKeyDown(KeyCode::KEY_CODE_SPACE)) {
		showShadowdepthmap = !showShadowdepthmap;
	}
	if (showShadowdepthmap) {
		glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		copyDepthRenderer.ShowDepth(m_shadowmapRenderer.GetDirectionDepthMap(0).GetID());
	}
	else {
		glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		/*copyDepthRenderer.ShowDepth(m_gbuffer.GetDepthTexture().GetID());*/
		//copyRenderer.CopyToScreen(m_lightAccmulatePass.GetLightBuffer().GetID());
		//copyRenderer.CopyToScreen(m_shadowmapRenderer.GetShadowmap().GetID());
		root->OnSurfaceRender(m_lightAccmulatePass.GetLightBuffer(), m_shadowmapRenderer.GetShadowmap());
	}


	//glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
	//glClearColor(0, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	///*copyDepthRenderer.ShowDepth(m_gbuffer.GetDepthTexture().GetID());*/
	////copyRenderer.CopyToScreen(m_lightAccmulatePass.GetLightBuffer().GetID());
	//root->OnSurfaceRender(m_lightAccmulatePass.GetLightBuffer());
}
