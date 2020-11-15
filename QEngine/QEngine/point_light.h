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
	glm::vec3 m_vAttenuationParams;

	
public:

	PointLight(glm::vec3 color,glm::vec3 params) :Light(LightType::POINT_LIGHT, color), m_vAttenuationParams(params) {
		
	}

	void SetupLightParams(Shader &shader) {
		Light::SetupLightParams(shader);
		shader.setFloat("Constant", m_vAttenuationParams.x);
		shader.setFloat("Linear", m_vAttenuationParams.y);
		shader.setFloat("Quadratic", m_vAttenuationParams.z);
	}

	LightType GetType() {
		return LightType::POINT_LIGHT;
	}
};

