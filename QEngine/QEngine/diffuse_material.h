#pragma once
#include "gl_material.h"
#include "gl_texture.h"
class DiffuseMaterial :public Material{
private:
	TexturePointer m_pDiffuseMap;
public:
	DiffuseMaterial();
	
	virtual void Use(const glm::mat4 &model);

	virtual void SetTexture(const std::string &key, TexturePointer tex);

	virtual MaterialPointer Clone();
};