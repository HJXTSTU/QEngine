#include "phong_material.h"
#include "gl_util.h"

PhongMaterial::PhongMaterial() {
	m_shader.LoadShader("defer_phong_shader.vs", "defer_phong_shader.frag");
	m_shader.use();
	m_shader.setInt(UNIFORM_DIFFUSE_MAP, UNIFORM_DIFFUSE_MAP_CODE);
	m_shader.setInt(UNIFORM_SPECULAR_MAP, UNIFORM_SPECULAR_MAP_CODE);
	m_shader.setInt(UNIFORM_NORMAL_MAP, UNIFORM_NORMAL_MAP_CODE);
	m_shader.setInt("LightBuffer", 3);
	m_shader.setInt("Shadowmap", 4);
}

void PhongMaterial::Use(const glm::mat4 &model) {
	Material::Use(model);
	if (m_pDiffuseMap) {
		m_pDiffuseMap->Use(UNIFORM_DIFFUSE_MAP_CODE);
		m_shader.setInt(UNIFORM_USE_DIFFUSE_MAP, 1);
	}
	else {
		m_shader.setInt(UNIFORM_USE_DIFFUSE_MAP, 0);
	}
	if (m_pSpecularMap) {
		m_pSpecularMap->Use(UNIFORM_SPECULAR_MAP_CODE);
		m_shader.setInt(UNIFORM_USE_SPECULAR_MAP, 1);
	}
	else {
		m_shader.setInt(UNIFORM_USE_SPECULAR_MAP, 0);
	}
	if (m_pNormalMap) {
		m_pNormalMap->Use(UNIFORM_NORMAL_MAP_CODE);
		m_shader.setInt(UNIFORM_USE_NORMAL_MAP, 1);
	}
	else {
		m_shader.setInt(UNIFORM_USE_NORMAL_MAP, 0);
	}
}

void PhongMaterial::Use(const glm::mat4 &model, const RenderTexture &lightBuffer) {
	Material::Use(model, lightBuffer);
	if (m_pDiffuseMap) {
		m_pDiffuseMap->Use(UNIFORM_DIFFUSE_MAP_CODE);
		m_shader.setInt(UNIFORM_USE_DIFFUSE_MAP, 1);
	}
	else {
		m_shader.setInt(UNIFORM_USE_DIFFUSE_MAP, 0);
	}
	if (m_pSpecularMap) {
		m_pSpecularMap->Use(UNIFORM_SPECULAR_MAP_CODE);
		m_shader.setInt(UNIFORM_USE_SPECULAR_MAP, 1);
	}
	else {
		m_shader.setInt(UNIFORM_USE_SPECULAR_MAP, 0);
	}
	if (m_pNormalMap) {
		m_pNormalMap->Use(UNIFORM_NORMAL_MAP_CODE);
		m_shader.setInt(UNIFORM_USE_NORMAL_MAP, 1);
	}
	else {
		m_shader.setInt(UNIFORM_USE_NORMAL_MAP, 0);
	}

	UseTexture(3, lightBuffer.GetID());
}

void PhongMaterial::Use(const glm::mat4 &model, const RenderTexture &lightBuffer, const RenderTexture &shadowmap) {
	Material::Use(model, lightBuffer, shadowmap);
	if (m_pDiffuseMap) {
		m_pDiffuseMap->Use(UNIFORM_DIFFUSE_MAP_CODE);
		m_shader.setInt(UNIFORM_USE_DIFFUSE_MAP, 1);
	}
	else {
		m_shader.setInt(UNIFORM_USE_DIFFUSE_MAP, 0);
	}
	if (m_pSpecularMap) {
		m_pSpecularMap->Use(UNIFORM_SPECULAR_MAP_CODE);
		m_shader.setInt(UNIFORM_USE_SPECULAR_MAP, 1);
	}
	else {
		m_shader.setInt(UNIFORM_USE_SPECULAR_MAP, 0);
	}
	if (m_pNormalMap) {
		m_pNormalMap->Use(UNIFORM_NORMAL_MAP_CODE);
		m_shader.setInt(UNIFORM_USE_NORMAL_MAP, 1);
	}
	else {
		m_shader.setInt(UNIFORM_USE_NORMAL_MAP, 0);
	}

	UseTexture(3, lightBuffer.GetID());
	UseTexture(4, shadowmap.GetID());
}


void PhongMaterial::Use(const glm::mat4 &model, Shader &shader) {
	Material::Use(model,shader);
	if (m_pDiffuseMap) {
		m_pDiffuseMap->Use(UNIFORM_DIFFUSE_MAP_CODE);
		shader.setInt(UNIFORM_USE_DIFFUSE_MAP, 1);
	}
	else {
		shader.setInt(UNIFORM_USE_DIFFUSE_MAP, 0);
	}
	if (m_pSpecularMap) {
		m_pSpecularMap->Use(UNIFORM_SPECULAR_MAP_CODE);
		shader.setInt(UNIFORM_USE_SPECULAR_MAP, 1);
	}
	else {
		shader.setInt(UNIFORM_USE_SPECULAR_MAP, 0);
	}
	if (m_pNormalMap) {
		m_pNormalMap->Use(UNIFORM_NORMAL_MAP_CODE);
		shader.setInt(UNIFORM_USE_NORMAL_MAP, 1);
	}
	else {
		shader.setInt(UNIFORM_USE_NORMAL_MAP, 0);
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