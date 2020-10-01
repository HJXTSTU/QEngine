#include "GBuffer.h"
GBuffer::GBuffer() {
	m_rtDepthTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
	m_framebuffer.AttachDepthStencilAttachment(m_rtDepthTexture);

	m_rtNormalTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGB16F, GL_RGB, GL_FLOAT);
	m_framebuffer.AttachColorAttachment(m_rtNormalTexture, 0);

	//m_rtTangentTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGB16F, GL_RGB, GL_FLOAT);
	//m_framebuffer.AttachColorAttachment(m_rtTangentTexture, 1);

	//m_rtBitangentTexture.Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGB16F, GL_RGB, GL_FLOAT);
	//m_framebuffer.AttachColorAttachment(m_rtBitangentTexture, 2);

	m_framebuffer.CompleteSetupFramebuffer(1);

	m_gbufferShader.LoadShader("gbuffer_shader.vs", "gbuffer_shader.frag");
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