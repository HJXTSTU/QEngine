#pragma once
#include "light.h"
#include "gl_screen_quad_vertex_array.h"
#include "gl_framebuffer.h"
#include "gl_render_texture.h"
#include "constant.h"
#include "gl_util.h"
class LightAccmulatePass {
private:
	Shader m_directionLightShader;
	Shader m_pointLightShader;
	Shader m_spotLightShader;
	
	Framebuffer m_lightAccmulateFramebuffer;
	RenderTexture m_rtLightBuffer;
	ScreenQuad m_screenQuad;
public:
	
	LightAccmulatePass() {
		m_rtLightBuffer.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);
		m_lightAccmulateFramebuffer.AttachColorAttachment(m_rtLightBuffer, 0);

		m_directionLightShader.LoadShader("screen_quad_vertex_shader_util.vs", "direction_light_accmulate_shader.frag");
		m_directionLightShader.use();
		m_directionLightShader.setInt("NormalBuffer", 0);
		m_directionLightShader.setInt("DepthBuffer", 1);
		
	}

	void ClearLightBuffer() {
		m_lightAccmulateFramebuffer.UseFramebuffer();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		m_lightAccmulateFramebuffer.UnUseFramebuffer();
	}
	
	void AccmulateLightShading(LightPointer light,const RenderTexture &normalBuffer, const RenderTexture &depthBuffer) {
		m_lightAccmulateFramebuffer.UseFramebuffer();
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_COLOR, GL_ONE, GL_SRC_ALPHA, GL_ONE);
		if (light->GetType() == LightType::DIRECTION) {
			m_directionLightShader.use();
			UseTexture(0, normalBuffer.GetID());
			UseTexture(1, depthBuffer.GetID());
			light->SetupLightParams(m_directionLightShader);
		}
		m_screenQuad.DrawScreenQuad();
		glDisable(GL_BLEND);
		m_lightAccmulateFramebuffer.UnUseFramebuffer();
	}

	RenderTexture &GetLightBuffer() {
		return m_rtLightBuffer;
	}
};