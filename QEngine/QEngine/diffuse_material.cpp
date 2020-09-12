#include "diffuse_material.h"
DiffuseMaterial::DiffuseMaterial() {
	m_shader.LoadShader("diffuse_shader.vs", "diffuse_shader.frag");
	m_shader.use();
	m_shader.setInt("diffuseMap", 0);
}

void DiffuseMaterial::Use(const glm::mat4 &model) {
	Material::Use(model);
	m_pDiffuseMap->Use(0);
}

void DiffuseMaterial::SetTexture(const std::string &key, TexturePointer tex) {
	if (key == "DIFFUSE_MAP") {
		m_pDiffuseMap = tex;
	}
}

MaterialPointer DiffuseMaterial::Clone() {
	MaterialPointer res = std::make_shared<DiffuseMaterial>();
	res->SetTexture("DIFFUSE_MAP", m_pDiffuseMap = m_pDiffuseMap);
	return res;
}