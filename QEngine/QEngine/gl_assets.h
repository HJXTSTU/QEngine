#pragma once
#include "gl_texture.h"
#include "gl_material.h"
#include "gl_model.h"
#include <memory>
#include <unordered_map>
class AssetsManager :public std::enable_shared_from_this<AssetsManager> {
private:
	std::unordered_map<std::string, TexturePointer> m_umTextures;
	std::unordered_map<std::string, MaterialPointer> m_umMaterials;

	std::unordered_map<std::string, ModelPointer> m_umModels;
public:
	void AddTexture(const std::string &id, TexturePointer pTexture);

	TexturePointer GetTexture(const std::string &id);

	void AddMaterial(const std::string &id, MaterialPointer pMaterial);

	MaterialPointer GetMaterial(const std::string &id);

	void AddModel(const std::string &id, ModelPointer model);

	ModelPointer GetModel(const std::string &id);
};