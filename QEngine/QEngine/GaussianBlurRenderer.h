#pragma once
#include "constant.h"
#include "PostprocessRenderer.h"
#include "gl_util.h"
#include "util.h"
class GaussianBlurRenderer :public PostprocessRenderer {
public:
	float sigma;
	int amount;
	int size;
	RenderTexture pingpongTexture[2];
public:

	GaussianBlurRenderer();


	void setSigma(float value);

	void setAmount(int value);

	void setMaxValue(float value);

	void setMinValue(float value);

	void setSize(int value);

	const RenderTexture& Render(const RenderTexture &image);
};