#pragma once
#include "light.h"
#include "constant.h"
#include "gl_framebuffer.h"
#include "gl_render_texture.h"
#include "gl_screen_quad_vertex_array.h"
#include "gl_bound_box.h"
#include "gl_util.h"


class PointLight:public Light {
private:
	glm::vec3 m_vPosition;
	glm::vec3 m_vAttenuationParams;

	RenderTexture tmp;
public:

	PointLight(glm::vec3 color,glm::vec3 position, glm::vec3 attenuationParams) :
		Light(LightType::POINT_LIGHT, color), m_vAttenuationParams(attenuationParams), m_vPosition(position) {
		
	}

	void SetupLightParams(Shader &shader) {
		Light::SetupLightParams(shader);
		shader.setVec3("Position", m_vPosition);
		shader.setFloat("Constant", m_vAttenuationParams.x);
		shader.setFloat("Linear", m_vAttenuationParams.y);
		shader.setFloat("Quadratic", m_vAttenuationParams.z);
	}

	glm::vec3 GetPosition() { return m_vPosition; }

	glm::vec3 GetAttenuationParams() { return m_vAttenuationParams; }

	LightType GetType() {
		return LightType::POINT_LIGHT;
	}


	virtual void ClearShadowmap() {

	}

	virtual const RenderTexture &GetShadowmap() {
		return tmp;
	}

	virtual void RenderShadowmap(shared_ptr<Object3D> root, Camera &camera, const RenderTexture &depthMap, const RenderTexture &normalMap) {
		
	}


	static LightPointer Create(glm::vec3 color, glm::vec3 position, glm::vec3 attenuationParams) {
		return LightPointer(new PointLight(color, position, attenuationParams));
	}
};

