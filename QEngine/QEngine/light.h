#pragma once
#include "shader.h"
#include "gl_object3D.h"
#include "gl_camera.h"
#include "gl_render_texture.h"
#include <memory>
enum LightType {
	DIRECTION,
	POINT_LIGHT,
	SPOT_LIGHT,
};

using namespace std;

class Light;
typedef shared_ptr<Light> LightPointer;

class Light {
protected:
	LightType m_eLightType;
	glm::vec3 m_vLightColor;

	float m_normalBias;
	float m_minBias;
	float m_maxBias;
public:

	Light(LightType type, glm::vec3 color);

	LightType GetType();

	void SetMaxBias(float value) { m_maxBias = value; }

	void SetMinBias(float value) { m_minBias = value; }

	void SetNormalBias(float value) { m_normalBias = value; }

	virtual void ClearShadowmap() = 0;

	virtual const RenderTexture &GetShadowmap() = 0;

	virtual void RenderShadowmap(shared_ptr<Object3D> root, Camera &camera, const RenderTexture &depthMap, const RenderTexture &normalMap) = 0;

	virtual void SetupLightParams(Shader &shader);
};