#include "DefferRenderer.h"
#include "gl_context.h"


DefferRenderer::DefferRenderer() {
	m_finalRenderTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_finalDepthTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
	m_finalFramebuffer.AttachColorAttachment(m_finalRenderTexture, 0);
	m_finalFramebuffer.AttachDepthStencilAttachment(m_finalDepthTexture);
}

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
		lights[i]->ClearShadowmap();
		lights[i]->RenderShadowmap(root, Context::mainCamera, m_gbuffer.GetDepthTexture(), m_gbuffer.GetVertexNormalTexture());
		m_shadowmapRenderer.MergeShadowmap(lights[i]->GetShadowmap());
	}


	static bool showDepthMap = false;
	if (Input::getKeyDown(KEY_CODE_SPACE)) {
		showDepthMap = !showDepthMap;
	}
	if (showDepthMap) {
		static unsigned int index = 0;
		if (Input::getKeyDown(KEY_CODE_N)) {
			index = (index + 1) % 4;
		}
		std::shared_ptr<DirectionLight> dirLight = dynamic_pointer_cast<DirectionLight>(lights[0]);
		copyDepthRenderer.ShowDepth(dirLight->GetShadowmap().GetID());
	}
	else {
		//m_gaussianBlurRenderer.setAmount(10);
		//m_gaussianBlurRenderer.setSigma(30.0f);
		//m_gaussianBlurRenderer.setSize(4.0f);

		//const RenderTexture &result = m_gaussianBlurRenderer.Render(m_shadowmapRenderer.GetShadowmap());

		m_finalFramebuffer.UseFramebuffer();
		glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		root->OnSurfaceRender(m_lightAccmulatePass.GetLightBuffer(), m_shadowmapRenderer.GetShadowmap());
		m_finalFramebuffer.UnUseFramebuffer();

		m_acesTonemappingRenderer.setAdaptedIlumination(1.0f);
		const RenderTexture &tonemappedImage = m_acesTonemappingRenderer.Render(m_finalRenderTexture);

		copyRenderer.CopyToScreen(tonemappedImage.GetID());
	}
}
