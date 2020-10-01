#pragma once
#include "gl_material.h"
class PhongMaterial:public Material {
private:
	TexturePointer m_pDiffuseMap;
	TexturePointer m_pSpecularMap;
	TexturePointer m_pNormalMap;
public:
	PhongMaterial();

	virtual void Use(const glm::mat4 &model);

	virtual void Use(const glm::mat4 &model, const RenderTexture &lightBuffer);

	virtual void SetTexture(const std::string &key, TexturePointer tex);

	virtual MaterialPointer Clone();
};