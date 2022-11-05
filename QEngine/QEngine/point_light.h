#pragma once
#include "light.h"
#include "constant.h"
#include "gl_framebuffer.h"
#include "gl_render_texture.h"
#include "gl_screen_quad_vertex_array.h"
#include "gl_bound_box.h"
#include "gl_util.h"
#include "gl_cube_map.h"
#include "input.h"


class PointLight:public Light {
private:
	glm::vec3 m_vPosition;
	glm::vec3 m_vAttenuationParams;

	Framebuffer m_shadowmapFramebuffer;
	RenderTexture m_rtShadowmap;
	Shader m_shadowmapShader;
	ScreenQuad m_screenQuad;

	Framebuffer m_fbDepthFramebuffer;
	CubeMap m_cmDepthMap;
	Shader m_depthShader;
public:

	PointLight(glm::vec3 color, glm::vec3 position, glm::vec3 attenuationParams);

	void SetupLightParams(Shader &shader);

	glm::vec3 GetPosition() { return m_vPosition; }

	glm::vec3 GetAttenuationParams() { return m_vAttenuationParams; }

	LightType GetType() {
		return LightType::POINT_LIGHT;
	}


	virtual void ClearShadowmap();

	virtual const RenderTexture& GetShadowmap();

	virtual void RenderShadowmap(shared_ptr<Object3D> root, Camera &camera, const RenderTexture &depthMap, const RenderTexture &normalMap);


	static LightPointer Create(glm::vec3 color, glm::vec3 position, glm::vec3 attenuationParams);
};

