#include "point_light.h"

PointLight::PointLight(glm::vec3 color, glm::vec3 position, glm::vec3 attenuationParams) :
	Light(LightType::POINT_LIGHT, color), m_vAttenuationParams(attenuationParams), m_vPosition(position) {
	m_rtShadowmap.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_shadowmapFramebuffer.AttachColorAttachment(m_rtShadowmap, 0);
	m_shadowmapShader.LoadShader("screen_quad_vertex_shader_util.vs", "point_light_shadow_map.frag");
	m_shadowmapShader.use();
	m_shadowmapShader.setInt("DepthBuffer", 0);
	m_shadowmapShader.setInt("NormalBuffer", 1);
	m_shadowmapShader.setInt("LightDepthBuffer", 2);

	m_cmDepthMap.Initialize(SHADOWMAP_DEFAULT_SIZE, SHADOWMAP_DEFAULT_SIZE, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	m_cmDepthMap.SetMinFilter(GL_NEAREST);
	m_cmDepthMap.SetMagFilter(GL_NEAREST);
	m_cmDepthMap.SetWrapS(GL_CLAMP_TO_EDGE);
	m_cmDepthMap.SetWrapT(GL_CLAMP_TO_EDGE);
	m_cmDepthMap.SetWrapR(GL_CLAMP_TO_EDGE);
	m_fbDepthFramebuffer.AttachDepthAttachment(m_cmDepthMap);
	m_depthShader.LoadShader("point_light_shadow.vs", "point_light_shadow.frag", "point_light_shadow.gs");
}

void PointLight::SetupLightParams(Shader &shader) {
	Light::SetupLightParams(shader);
	shader.setVec3("Position", m_vPosition);
	shader.setFloat("Constant", m_vAttenuationParams.x);
	shader.setFloat("Linear", m_vAttenuationParams.y);
	shader.setFloat("Quadratic", m_vAttenuationParams.z);
}


void PointLight::ClearShadowmap() {
	m_shadowmapFramebuffer.UseFramebuffer();
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	m_shadowmapFramebuffer.UnUseFramebuffer();
}

const RenderTexture& PointLight::GetShadowmap() {
	return m_rtShadowmap;
}

void PointLight::RenderShadowmap(shared_ptr<Object3D> root, Camera &camera, const RenderTexture &depthMap, const RenderTexture &normalMap) {
	GLfloat aspect = (GLfloat)SHADOWMAP_DEFAULT_SIZE / (GLfloat)SHADOWMAP_DEFAULT_SIZE;
	GLfloat _near = SHADOWMAP_POINT_LIGHT_NEAR;
	GLfloat _far = SHADOWMAP_POINT_LIGHT_FAR;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, _near, _far);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(m_vPosition, m_vPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(m_vPosition, m_vPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(m_vPosition, m_vPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(m_vPosition, m_vPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(m_vPosition, m_vPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(m_vPosition, m_vPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	m_fbDepthFramebuffer.UseFramebuffer();
	glViewport(0, 0, SHADOWMAP_DEFAULT_SIZE, SHADOWMAP_DEFAULT_SIZE);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_depthShader.use();
	m_depthShader.setFloat("FarPlane", _far);
	m_depthShader.setVec3("Position", m_vPosition);
	for (int i = 0; i < 6; i++)
		m_depthShader.setMat4(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
	root->OnShadowmapRender(m_depthShader);
	m_fbDepthFramebuffer.UnUseFramebuffer();


	m_shadowmapFramebuffer.UseFramebuffer();
	m_shadowmapShader.use();
	UseTexture(0, depthMap.GetID());
	UseTexture(1, normalMap.GetID());
	UseCubemap(2, m_cmDepthMap.GetID());
	m_shadowmapShader.setVec3("Position", m_vPosition);
	m_shadowmapShader.setFloat("FarPlane", _far);

	m_shadowmapShader.setFloat("NormalBias", m_normalBias);
	m_shadowmapShader.setVec2("LightBias", vec2(m_minBias, m_maxBias));
	m_shadowmapShader.setFloat("Intensity", m_intensity);
	this->SetupLightParams(m_shadowmapShader);
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	m_screenQuad.DrawScreenQuad();
	m_shadowmapFramebuffer.UnUseFramebuffer();
}


LightPointer PointLight::Create(glm::vec3 color, glm::vec3 position, glm::vec3 attenuationParams) {
	return LightPointer(new PointLight(color, position, attenuationParams));
}