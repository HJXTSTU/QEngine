#include "GBuffer.h"
GBuffer::GBuffer() {
	m_rtDepthTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
	m_framebuffer.AttachDepthStencilAttachment(m_rtDepthTexture);

	m_rtVertexNormalTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGB16F, GL_RGB, GL_FLOAT);
	m_framebuffer.AttachColorAttachment(m_rtVertexNormalTexture, 0);

	m_rtNormalTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGB16F, GL_RGB, GL_FLOAT);
	m_framebuffer.AttachColorAttachment(m_rtNormalTexture, 1);

	m_framebuffer.CompleteSetupFramebuffer(2);

	m_gbufferShader.LoadShader("gbuffer_shader.vs", "gbuffer_shader.frag");
	m_gbufferShader.use();
	m_gbufferShader.setInt(UNIFORM_NORMAL_MAP, UNIFORM_NORMAL_MAP_CODE);
}

void GBuffer::Render(const shared_ptr<Object3D> world) {
	m_framebuffer.UseFramebuffer();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	m_gbufferShader.use();
	world->OnGBufferRender(m_gbufferShader);
	m_framebuffer.UnUseFramebuffer();
}