#include "GaussianBlurRenderer.h"

GaussianBlurRenderer::GaussianBlurRenderer() :PostprocessRenderer("gaussianblur.frag"){
	m_shader.use();
	m_shader.setInt("image", 0);
	for (int i = 0; i < 2; i++) {
		pingpongTexture[i].Initialize(SRC_WIDTH, SRC_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	}
}


void GaussianBlurRenderer::setSigma(float value) { sigma = abs(value); }

void GaussianBlurRenderer::setAmount(int value) { amount = abs(value); }

void GaussianBlurRenderer::setSize(int value) { size = value; }

const RenderTexture& GaussianBlurRenderer::Render(const RenderTexture &image) {
	static float e = glm::e<float>();
	static float pi = glm::pi<float>();
	m_shader.use();

	float sigma2 = sigma * sigma;
	float left = 1.0f / sqrt((2 * sigma2*pi));
	float weight[10];
	float sum = 0.0f;
	for (int i = 0; i < min(size, 10); i++) {
		weight[i] = left * pow(e, -(i*i) / (2 * sigma2));
		if(i==0)
			sum += weight[i];
		else
			sum += 2*weight[i];

	}

	for (int i = 0; i < min(size, 10); i++) {
		weight[i] /= sum;
		char name[64];
		sprintf(name, "weight[%d]", i);
		m_shader.setFloat(name, weight[i]);
	}
	m_shader.setInt("size", size);


	bool firstRender = true;
	bool horizontal = true;
	for (int i = 0; i < amount; i++) {
		m_framebuffer.AttachColorAttachment(pingpongTexture[1 - horizontal], 0);
		UseTexture(0, firstRender ? image.GetID() : pingpongTexture[horizontal].GetID());
		m_shader.setBool("horizontal", horizontal);
		m_framebuffer.UseFramebuffer();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		m_screen.DrawScreenQuad();
		m_framebuffer.UnUseFramebuffer();
		horizontal = !horizontal;
		firstRender = false;
	}
	return pingpongTexture[amount % 2];
}

