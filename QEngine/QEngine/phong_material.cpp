#include "phong_material.h"

PhongMaterial::PhongMaterial() {
	m_shader.LoadShader("phong_shader.vs", "phong_shader.frag");
	m_shader.use();
	m_shader.setInt("diffuseMap", 0);
	m_shader.setInt("specularMap", 1);
	m_shader.setInt("normalMap", 2);
}

void PhongMaterial::Use(const glm::mat4 &model) {
	Material::Use(model);
	if (m_pDiffuseMap) {
		m_pDiffuseMap->Use(0);
		m_shader.setInt("UseDiffuseMap", 1);
	}
	else {
		m_shader.setInt("UseDiffuseMap", 0);
	}
	if (m_pSpecularMap) {
		m_pSpecularMap->Use(1);
		m_shader.setInt("UseSpecularMap", 1);
	}
	else {
		m_shader.setInt("UseSpecularMap", 0);
	}
	if (m_pNormalMap) {
		m_pNormalMap->Use(2);
		m_shader.setInt("UseNormalMap", 1);
	}
	else {
		m_shader.setInt("UseNormalMap", 0);
	}
}

void PhongMaterial::SetTexture(const std::string &key, TexturePointer tex) {
	if (key == "DIFFUSE_MAP") {
		m_pDiffuseMap = tex;
	}
	if (key == "SPECULAR_MAP") {
		m_pSpecularMap = tex;
	}

	if (key == "NORMAL_MAP") {
		m_pNormalMap = tex;
	}
}

MaterialPointer PhongMaterial::Clone() {
	MaterialPointer res = std::make_shared<PhongMaterial>();
	res->SetTexture("DIFFUSE_MAP", m_pDiffuseMap);
	res->SetTexture("SPECULAR_MAP", m_pSpecularMap);
	res->SetTexture("NORMAL_MAP", m_pNormalMap);
	return res;
}