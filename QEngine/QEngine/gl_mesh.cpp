#include "gl_mesh.h"

Mesh::Mesh(GeometryPointer geometry, MaterialPointer material) :pGeometry(geometry),pMaterial(material)
{
	this->vertexArray.FillGeometryData(geometry);
}

void Mesh::OnGBufferRender(Shader& gbufferShader)
{
	glm::mat4 model = this->transform.MatrixWorld();
	gbufferShader.use();
	this->pMaterial->Use(model, gbufferShader);
	this->vertexArray.DrawElement();
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnGBufferRender(gbufferShader);
	}
}

void Mesh::OnSurfaceRender()
{

	this->pMaterial->Use(this->transform.MatrixWorld());

	this->vertexArray.DrawElement();

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnSurfaceRender();
	}
}

void Mesh::OnSurfaceRender(const RenderTexture &lightBuffer) {
	this->pMaterial->Use(this->transform.MatrixWorld(), lightBuffer);

	this->vertexArray.DrawElement();

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnSurfaceRender(lightBuffer);
	}
}

void Mesh::OnSurfaceRender(const RenderTexture &lightBuffer, const RenderTexture &shadowmap) {
	this->pMaterial->Use(this->transform.MatrixWorld(), lightBuffer, shadowmap);

	this->vertexArray.DrawElement();

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnSurfaceRender(lightBuffer, shadowmap);
	}
}

void Mesh::OnShadowmapRender(Shader& shader) {
	glm::mat4 model = this->transform.MatrixWorld();
	shader.setMat4("model", model);
	this->vertexArray.DrawElement();
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnShadowmapRender(shader);
	}
}

ObjectType Mesh::GetType()
{
	return ObjectType::MESH_OBJECT;
}
