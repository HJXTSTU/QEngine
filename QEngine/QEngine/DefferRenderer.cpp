#include "DefferRenderer.h"
#include "gl_context.h"


DefferRenderer::DefferRenderer() {
	m_finalRenderTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_finalFramebuffer.AttachColorAttachment(m_finalRenderTexture,0);
	m_finalFramebuffer.AttachDepthStencilAttachment();
}

void DefferRenderer::OnRender(BaseWorldPointer pWorld) {
	const shared_ptr<Object3D> root = pWorld->Root();
	m_gbuffer.Render(root);

	m_lightAccmulatePass.ClearLightBuffer();
	vector<LightPointer> lights = pWorld->GetLights();
	for (int i = 0; i < lights.size(); i++) {
		m_lightAccmulatePass.AccmulateLightShading(lights[i], m_gbuffer.GetNormalTexture(), m_gbuffer.GetDepthTexture());
	}

	/*m_shadowmapRenderer.Clear();
	for (int i = 0; i < lights.size(); i++) {
		lights[i]->ClearShadowmap();
		lights[i]->RenderShadowmap(root, Context::mainCamera, m_gbuffer.GetDepthTexture(), m_gbuffer.GetVertexNormalTexture());
		m_shadowmapRenderer.MergeShadowmap(lights[i]->GetShadowmap(), lights.size());
	}*/

	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	/*m_gaussianBlurRenderer.setAmount(6);
	m_gaussianBlurRenderer.setSigma(0.5f);
	m_gaussianBlurRenderer.setMaxValue(1.0f);
	m_gaussianBlurRenderer.setMinValue(0.0f);

	const RenderTexture &result = m_gaussianBlurRenderer.Render(m_shadowmapRenderer.GetShadowmap())*/;
	root->OnSurfaceRender(m_lightAccmulatePass.GetLightBuffer());
}
