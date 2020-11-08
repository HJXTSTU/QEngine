#pragma once
#include "constant.h"
#include "PostprocessRenderer.h"
#include "gl_util.h"
#include "util.h"
class GaussianBlurRenderer :public PostprocessRenderer {
public:
	float sigma;
	float maxValue;
	float minValue;
	int amount;
	RenderTexture pingpongTexture[2];
public:

	GaussianBlurRenderer();


	void setSigma(float value);

	void setAmount(int value);

	void setMaxValue(float value);

	void setMinValue(float value);

	const RenderTexture& Render(const RenderTexture &image);
};