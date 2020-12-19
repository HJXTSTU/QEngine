#include "direction_light.h"


DirectionLight::DirectionLight(glm::vec3 color, glm::vec3 direction)
	:Light(LightType::DIRECTION, color), m_vLightDirection(direction) {
	m_rtShadowmap.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);

	m_shadowmapFramebuffer.AttachColorAttachment(m_rtShadowmap, 0);

	m_shadowmapShader.LoadShader("screen_quad_vertex_shader_util.vs", "direction_light_shadow_map_pcss.frag");
	m_shadowmapShader.use();
	m_shadowmapShader.setInt("DepthBuffer", 0);
	m_shadowmapShader.setInt("NormalBuffer", 1);
	m_shadowmapShader.setInt("LightDepthBuffer1", 2);
	m_shadowmapShader.setInt("LightDepthBuffer2", 3);
	m_shadowmapShader.setInt("LightDepthBuffer3", 4);
	m_shadowmapShader.setInt("LightDepthBuffer4", 5);

	m_depthShader.LoadShader("direction_light_shadow.vs", "direction_light_shadow.frag");
	GLfloat border[4] = { 1.0f,1.0f,1.0f,1.0f };
	for (int i = 0; i < SHADOWMAP_CASACADE_COUNT; i++) {
		m_rtDepthMap[i].Initialize(SHADOWMAP_DEFAULT_SIZE, SHADOWMAP_DEFAULT_SIZE, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
		m_rtDepthMap[i].SetMagFilter(GL_LINEAR);
		m_rtDepthMap[i].SetMinFilter(GL_LINEAR);
		m_rtDepthMap[i].SetWrapS(GL_CLAMP_TO_BORDER);
		m_rtDepthMap[i].SetWrapT(GL_CLAMP_TO_BORDER);
		m_rtDepthMap[i].SetBorderColor(border);
	}

}

void DirectionLight::SetupLightParams(Shader &shader) {
	Light::SetupLightParams(shader);
	shader.setVec3("LightDirection", m_vLightDirection);
}

void DirectionLight::ClearShadowmap() {
	m_shadowmapFramebuffer.UseFramebuffer();
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	m_shadowmapFramebuffer.UnUseFramebuffer();
}

const RenderTexture& DirectionLight::GetShadowmap() { return m_rtShadowmap; }

void DirectionLight::RenderShadowmap(shared_ptr<Object3D> root, Camera &camera, const RenderTexture &depthMap, const RenderTexture &normalMap) {

	const glm::vec3 LIGHT_DIR = glm::normalize(m_vLightDirection);
	const glm::vec3 LIGHT_RIGHT = glm::normalize(glm::cross(LIGHT_DIR, glm::vec3(0, 1, 0)));
	const glm::vec3 LIGHT_UP = glm::normalize(glm::cross(LIGHT_RIGHT, LIGHT_DIR));
	const float EYE_DISTANCE = SHADOWMAP_EYE_DISTANCE;

	const float OUTER_WIDTH = SHADOWMAP_OUTER_WIDTH;


	vector<glm::vec2> lightSizes;
	vector<glm::mat4> lightSpaceMatrixs;
	vector<glm::vec2> nearFarPlanes;
	vector<float> cascadeSplits{ 
		SHADOWMAP_CASCADE_SPLITS_0,
		SHADOWMAP_CASCADE_SPLITS_1,
		SHADOWMAP_CASCADE_SPLITS_2,
		SHADOWMAP_CASCADE_SPLITS_3
	};
	vector<glm::vec2> screenPoints = {
		glm::vec2(-1.0f,-1.0f),
		glm::vec2(-1.0f, 1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2( 1.0f,-1.0f)
	};

	float nearDistance = 0.0f;
	float farDistance = cascadeSplits[0];
	for (int i = 0; i < SHADOWMAP_CASACADE_COUNT; i++) {
		float minX = 999999.9f, minY = 999999.9f, minZ = 999999.9f;
		float maxX = -999999.9f, maxY = -999999.9f, maxZ = -999999.9f;

		for (int j = 0; j < screenPoints.size(); j++) {
			glm::vec3 nearPlaneCorner = camera.ScreenToWorld(screenPoints[j], 0.0f);
			glm::vec3 farPlaneCorner  = camera.ScreenToWorld(screenPoints[j], 1.0f);
			glm::vec3 nearCorner = nearPlaneCorner + (farPlaneCorner - nearPlaneCorner)*nearDistance;
			glm::vec3  farCorner = nearPlaneCorner + (farPlaneCorner - nearPlaneCorner)*farDistance;
			minX = min(min(minX, nearCorner.x), farCorner.x);
			minY = min(min(minY, nearCorner.y), farCorner.y);
			minZ = min(min(minZ, nearCorner.z), farCorner.z);

			maxX = max(max(maxX, nearCorner.x), farCorner.x);
			maxY = max(max(maxY, nearCorner.y), farCorner.y);
			maxZ = max(max(maxZ, nearCorner.z), farCorner.z);
		}

		BoundBox boundBox(minX, minY, minZ, maxX, maxY, maxZ);

		vec3 centerPoint = boundBox.Center();

		vec3 eye = centerPoint - LIGHT_DIR * EYE_DISTANCE;
		vec3 dir = LIGHT_DIR;
		vec3 up = vec3(0, 1, 0);
		vec3 right = normalize(cross(dir, up));
		up = normalize(cross(right, dir));

		glm::mat4 view = glm::lookAt(eye, eye + dir, up);
		minX = 999999.9f, minY = 999999.9f, minZ = 999999.9f;
		maxX = -999999.9f, maxY = -999999.9f, maxZ = -999999.9f;
		for (int j = 0; j < screenPoints.size(); j++) {
			glm::vec3 nearPlaneCorner = camera.ScreenToWorld(screenPoints[j], 0.0f);
			glm::vec3 farPlaneCorner = camera.ScreenToWorld(screenPoints[j], 1.0f);
			glm::vec3 nearCorner = nearPlaneCorner + (farPlaneCorner - nearPlaneCorner)*nearDistance;
			glm::vec3  farCorner = nearPlaneCorner + (farPlaneCorner - nearPlaneCorner)*farDistance;
			nearCorner = view * vec4(nearCorner, 1.0f);
			farCorner = view * vec4(farCorner, 1.0f);
			minX = min(min(minX, nearCorner.x), farCorner.x);
			minY = min(min(minY, nearCorner.y), farCorner.y);
			minZ = min(min(minZ, nearCorner.z), farCorner.z);

			maxX = max(max(maxX, nearCorner.x), farCorner.x);
			maxY = max(max(maxY, nearCorner.y), farCorner.y);
			maxZ = max(max(maxZ, nearCorner.z), farCorner.z);
		}

		glm::mat4 projection = glm::ortho(minX - OUTER_WIDTH, maxX + OUTER_WIDTH, minY - OUTER_WIDTH, maxY + OUTER_WIDTH, -maxZ - OUTER_WIDTH, -minZ + OUTER_WIDTH);
		m_depthShader.use();
		m_depthShader.setMat4("lightSpaceMatrix", projection*view);

		lightSizes.push_back(vec2(maxX + 2 * OUTER_WIDTH - minX, maxY + 2 * OUTER_WIDTH - minY));
		lightSpaceMatrixs.push_back(projection*view);
		nearFarPlanes.push_back(vec2(-maxZ - OUTER_WIDTH, -minZ + OUTER_WIDTH));

		m_framebuffer.AttachDepthStencilAttachment(m_rtDepthMap[i]);
		m_framebuffer.UseFramebuffer();
		glViewport(0, 0, SHADOWMAP_DEFAULT_SIZE, SHADOWMAP_DEFAULT_SIZE);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glCullFace(GL_FRONT);
		root->OnShadowmapRender(m_depthShader);
		glCullFace(GL_BACK);
		m_framebuffer.UnUseFramebuffer();

		nearDistance = i < SHADOWMAP_CASACADE_COUNT ? cascadeSplits[i] : 0.0f;
		farDistance = i + 1 < SHADOWMAP_CASACADE_COUNT ? cascadeSplits[i + 1] : 0.0f;
	}

	m_shadowmapShader.use();
	m_shadowmapShader.setVec3("LightDirection", LIGHT_DIR);
	m_shadowmapShader.setFloat("CameraFar", CAMERA_FAR);
	m_shadowmapShader.setFloat("CameraNear", CAMERA_NEAR);
	m_shadowmapShader.setFloat("CascadeSplits[0]", cascadeSplits[0]);
	m_shadowmapShader.setFloat("CascadeSplits[1]", cascadeSplits[1]);
	m_shadowmapShader.setFloat("CascadeSplits[2]", cascadeSplits[2]);
	m_shadowmapShader.setFloat("CascadeSplits[3]", cascadeSplits[3]);

	UseTexture(0, depthMap.GetID());
	UseTexture(1, normalMap.GetID());
	UseTexture(2, m_rtDepthMap[0].GetID());
	m_shadowmapShader.setMat4("LightSpaceMatrix1", lightSpaceMatrixs[0]);
	m_shadowmapShader.setVec2("NearFarPlane1", nearFarPlanes[0]);
	m_shadowmapShader.setVec2("LightSize1", lightSizes[0]);

	UseTexture(3, m_rtDepthMap[1].GetID());
	m_shadowmapShader.setMat4("LightSpaceMatrix2", lightSpaceMatrixs[1]);
	m_shadowmapShader.setVec2("NearFarPlane2", nearFarPlanes[1]);
	m_shadowmapShader.setVec2("LightSize2", lightSizes[1]);

	UseTexture(4, m_rtDepthMap[2].GetID());
	m_shadowmapShader.setMat4("LightSpaceMatrix3", lightSpaceMatrixs[2]);
	m_shadowmapShader.setVec2("NearFarPlane3", nearFarPlanes[2]);
	m_shadowmapShader.setVec2("LightSize3", lightSizes[2]);

	UseTexture(5, m_rtDepthMap[3].GetID());
	m_shadowmapShader.setMat4("LightSpaceMatrix4", lightSpaceMatrixs[3]);
	m_shadowmapShader.setVec2("NearFarPlane4", nearFarPlanes[3]);
	m_shadowmapShader.setVec2("LightSize4", lightSizes[3]);

	m_shadowmapShader.setFloat("NormalBias", m_normalBias);
	m_shadowmapShader.setVec2("LightBias", vec2(m_minBias, m_maxBias));
	m_shadowmapFramebuffer.UseFramebuffer();
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
	m_screenQuad.DrawScreenQuad();
	m_shadowmapFramebuffer.UnUseFramebuffer();
}

const RenderTexture& DirectionLight::GetDepthMap(GLuint index) {
	return m_rtDepthMap[index];
}

LightPointer DirectionLight::Create(glm::vec3 color, glm::vec3 direction) {
	return LightPointer(new DirectionLight(color, direction));
}


