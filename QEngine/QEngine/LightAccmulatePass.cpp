#include "LightAccmulatePass.h"

LightAccmulatePass::LightAccmulatePass() {
	m_rtLightBuffer.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_lightAccmulateFramebuffer.AttachColorAttachment(m_rtLightBuffer, 0);

	m_directionLightShader.LoadShader("screen_quad_vertex_shader_util.vs", "direction_light_accmulate_shader.frag");
	m_directionLightShader.use();
	m_directionLightShader.setInt("NormalBuffer", 0);
	m_directionLightShader.setInt("DepthBuffer", 1);

	m_pointLightShader.LoadShader("screen_quad_vertex_shader_util.vs", "point_light_accmulate_shader.frag");
	m_pointLightShader.use();
	m_pointLightShader.setInt("NormalBuffer", 0);
	m_pointLightShader.setInt("DepthBuffer", 1);
}

void LightAccmulatePass::ClearLightBuffer() {
	m_lightAccmulateFramebuffer.UseFramebuffer();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	m_lightAccmulateFramebuffer.UnUseFramebuffer();
}

void LightAccmulatePass::AccmulateLightShading(LightPointer light, const RenderTexture &normalBuffer, const RenderTexture &depthBuffer) {
	m_lightAccmulateFramebuffer.UseFramebuffer();
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_COLOR, GL_ONE, GL_SRC_ALPHA, GL_ONE);
	Shader *lightShader = NULL;
	if (light->GetType() == LightType::DIRECTION) {
		lightShader = &m_directionLightShader;
	}
	else if (light->GetType() == LightType::POINT_LIGHT) {
		lightShader = &m_pointLightShader;
	}
	lightShader->use();
	UseTexture(0, normalBuffer.GetID());
	UseTexture(1, depthBuffer.GetID());
	light->SetupLightParams(*lightShader);

	m_screenQuad.DrawScreenQuad();
	glDisable(GL_BLEND);
	m_lightAccmulateFramebuffer.UnUseFramebuffer();
}

RenderTexture & LightAccmulatePass::GetLightBuffer() {
	return m_rtLightBuffer;
}
