#pragma once
#include "gl_framebuffer.h"
#include "gl_render_texture.h"
#include "constant.h"
#include "gl_object3D.h"
#include "light.h"
#include "gl_camera.h"
#include "direction_light.h"
#include "shader.h"
#include "gl_screen_quad_vertex_array.h"

class ShadowmapRenderer {
private:
	Framebuffer m_shadowmapFramebuffer;
	RenderTexture m_rtShadowmap;
	ScreenQuad m_screenQuad;

	Framebuffer m_framebuffer;
	Shader m_directionLightDepthShader;
	Shader m_pssmShader;
	RenderTexture m_rtDirectionDepthMap[3];
private:
	void renderDirectionLightShadow(shared_ptr<Object3D> root, LightPointer light, Camera &camera,
		const RenderTexture &depthMap, const RenderTexture &normalMap) {
		//	取摄像机前方向并压缩到水平面
		glm::vec3 front = camera.Front;
		front.y = 0.0f;
		front = glm::normalize(front);

		//	取摄像机位置
		glm::vec3 pos = camera.Position;

		//	光源照射方向
		DirectionLightPointer dirLight = dynamic_pointer_cast<DirectionLight>(light);
		const glm::vec3 LIGHT_DIR = glm::normalize(dirLight->GetDirection());
		const glm::vec3 LIGHT_RIGHT = glm::normalize(glm::cross(LIGHT_DIR, glm::vec3(0, 1, 0)));
		const glm::vec3 LIGHT_UP = glm::normalize(glm::cross(LIGHT_RIGHT, LIGHT_DIR));

		//	计算三级视锥在极线平面上的坐标
		const glm::vec3 UP(0, 1, 0);
		const glm::vec3 RIGHT = glm::normalize(glm::cross(front, UP));
		const glm::vec3 FRONT = front;
		const float EYE_DISTANCE = 100.0f;

		const float TAN_HALF_FOV = glm::tan(glm::radians(camera.Zoom / 2.0f));
		const float INV_ASPECT = SRC_HEIGHT / SRC_WIDTH;
		const float INTERVAL = 10.0f;
		const float OUTER_WIDTH = 10.0f;

		const glm::vec3 ORIGIN = pos;

		float nearDistance = 0.0f;
		float farDistance = 10.0f;
		vector<glm::mat4> lightSpaceMatrixs;
		for (int i = 0; i < 3; i++) {
			float nearHeight = nearDistance * TAN_HALF_FOV*2.0f;
			float farHeight = farDistance * TAN_HALF_FOV*2.0f;

			float nearWidth = nearHeight / INV_ASPECT;
			float farWidth = farHeight / INV_ASPECT;

			glm::vec3 nearCenter = ORIGIN + FRONT * nearDistance;
			glm::vec3 farCenter = ORIGIN + FRONT * farDistance;

			glm::vec3 nearLeft = nearCenter - RIGHT * nearWidth;
			glm::vec3 nearRight = nearCenter + RIGHT * nearWidth;

			glm::vec3 farLeft = farCenter - RIGHT * farWidth;
			glm::vec3 farRight = farCenter + RIGHT * farWidth;

			glm::vec3 centerPoint = ORIGIN + 0.5f*(farDistance + nearDistance);

			glm::mat4 view = glm::lookAt(centerPoint - LIGHT_DIR * EYE_DISTANCE, centerPoint, LIGHT_UP);

			glm::vec3 centerPoiintInView = vec3(view * vec4(centerPoint, 1.0f));
			glm::vec3 nearLeftInView = vec3(view * vec4(nearLeft, 1.0f));
			glm::vec3 nearRightInView = vec3(view * vec4(nearRight, 1.0f));
			glm::vec3 farLeftInView = vec3(view * vec4(farLeft, 1.0f));
			glm::vec3 farRightInView = vec3(view * vec4(farRight, 1.0f));

			float right = glm::max(glm::max(glm::max(nearLeftInView.x, nearRightInView.x), farLeftInView.x), farRightInView.x) + OUTER_WIDTH;
			float left = glm::min(glm::min(glm::min(nearLeftInView.x, nearRightInView.x), farLeftInView.x), farRightInView.x) - OUTER_WIDTH;
			float top = glm::max(glm::max(glm::max(nearLeftInView.y, nearRightInView.y), farLeftInView.y), farRightInView.y) + OUTER_WIDTH;
			float bottom = glm::min(glm::min(glm::min(nearLeftInView.y, nearRightInView.y), farLeftInView.y), farRightInView.y) - OUTER_WIDTH;

			glm::mat4 projection = glm::ortho(left,right,bottom,right, 0.1f, 500.0f);
			m_directionLightDepthShader.use();
			m_directionLightDepthShader.setMat4("lightSpaceMatrix", projection*view);
			lightSpaceMatrixs.push_back(projection*view);

			m_framebuffer.AttachDepthStencilAttachment(m_rtDirectionDepthMap[i]);
			m_framebuffer.UseFramebuffer();
			glViewport(0, 0, 4096 * (3 - i), 4096 * (3 - i));
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glCullFace(GL_FRONT);
			root->OnShadowmapRender(m_directionLightDepthShader);
			glCullFace(GL_BACK);
			m_framebuffer.UnUseFramebuffer();

			nearDistance += INTERVAL;
			farDistance += INTERVAL;
		}

		m_pssmShader.use();
		m_pssmShader.setFloat("Interval", INTERVAL);
		m_pssmShader.setVec3("LightDirection", LIGHT_DIR);
		UseTexture(0, depthMap.GetID());
		UseTexture(1, normalMap.GetID());
		UseTexture(2, m_rtDirectionDepthMap[0].GetID());
		m_pssmShader.setMat4("LightSpaceMatrix1", lightSpaceMatrixs[0]);
		UseTexture(3, m_rtDirectionDepthMap[1].GetID());
		m_pssmShader.setMat4("LightSpaceMatrix2", lightSpaceMatrixs[1]);
		UseTexture(4, m_rtDirectionDepthMap[2].GetID());
		m_pssmShader.setMat4("LightSpaceMatrix3", lightSpaceMatrixs[2]);

		m_shadowmapFramebuffer.UseFramebuffer();
		glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
		m_screenQuad.DrawScreenQuad();
		m_shadowmapFramebuffer.UnUseFramebuffer();
	}

public:

	const RenderTexture &GetShadowmap() { return this->m_rtShadowmap; }
	
	void Clear() {
		m_shadowmapFramebuffer.UseFramebuffer();
		glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		m_screenQuad.DrawScreenQuad();
		m_shadowmapFramebuffer.UnUseFramebuffer();
	}
	ShadowmapRenderer() {

		m_rtShadowmap.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);

		m_shadowmapFramebuffer.AttachColorAttachment(m_rtShadowmap, 0);

		m_directionLightDepthShader.LoadShader("direction_light_shadow.vs", "direction_light_shadow.frag");
		m_pssmShader.LoadShader("screen_quad_vertex_shader_util.vs","direction_light_shadow_map.frag");
		m_pssmShader.use();
		m_pssmShader.setInt("DepthBuffer",0);
		m_pssmShader.setInt("NormalBuffer",1);
		m_pssmShader.setInt("LightDepthBuffer1",2);
		m_pssmShader.setInt("LightDepthBuffer2",3);
		m_pssmShader.setInt("LightDepthBuffer3",4);
		for (int i = 0; i < 3; i++) {
			m_rtDirectionDepthMap[i].Initialize(4096*(3-i), 4096 * (3 - i), GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
		}
	}

	void OnRender(shared_ptr<Object3D> root, LightPointer light, Camera &camera, const RenderTexture &depthMap, const RenderTexture &normalMap) {
		switch (light->GetType()) {
		case LightType::DIRECTION:
			renderDirectionLightShadow(root, light, camera, depthMap, normalMap);
			break;
		}

	}

	const RenderTexture& GetDirectionDepthMap(int index) { return m_rtDirectionDepthMap[index]; }
};

