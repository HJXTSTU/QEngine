#pragma once
#include "light.h"
#include "constant.h"
#include "gl_framebuffer.h"
#include "gl_render_texture.h"
#include "gl_screen_quad_vertex_array.h"
#include "gl_bound_box.h"
#include "gl_util.h"

class DirectionLight;
typedef shared_ptr<DirectionLight> DirectionLightPointer;

class DirectionLight :public Light {
private:
	glm::vec3 m_vLightDirection;

	Framebuffer m_shadowmapFramebuffer;
	RenderTexture m_rtShadowmap;
	RenderTexture m_rtDepthMap[4];
	ScreenQuad m_screenQuad;

	Framebuffer m_framebuffer;
	Shader m_depthShader;
	Shader m_shadowmapShader;
public:
	DirectionLight(glm::vec3 color, glm::vec3 direction);

	const RenderTexture& GetDepthMap(GLuint index);

	virtual const RenderTexture& GetShadowmap();

	virtual void SetupLightParams(Shader &shader);

	virtual void ClearShadowmap();

	virtual void RenderShadowmap(shared_ptr<Object3D> root, Camera &camera, const RenderTexture &depthMap, const RenderTexture &normalMap);

	glm::vec3 GetDirection() { return m_vLightDirection; }

	static LightPointer Create(glm::vec3 color, glm::vec3 direction);
};