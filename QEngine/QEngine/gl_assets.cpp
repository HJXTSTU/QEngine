#include "gl_assets.h"

void AssetsManager::AddTexture(const std::string &id, TexturePointer pTexture){
	m_umTextures[id] = pTexture;
}

TexturePointer AssetsManager::GetTexture(const std::string &id) {
	if (m_umTextures.find(id) == m_umTextures.end())return TexturePointer(NULL);
	return m_umTextures[id];
}

void AssetsManager::AddMaterial(const std::string &id, MaterialPointer pMaterial) {
	m_umMaterials[id] = pMaterial;
}

MaterialPointer AssetsManager::GetMaterial(const std::string &id) {
	if (m_umMaterials.find(id) == m_umMaterials.end())return MaterialPointer(NULL);
	return m_umMaterials[id];
}
 