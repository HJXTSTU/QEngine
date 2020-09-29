#include "gl_mesh.h"

Mesh::Mesh(GeometryPointer geometry, MaterialPointer material) :pGeometry(geometry),pMaterial(material)
{
	this->vertexArray.FillGeometryData(geometry);
}

void Mesh::OnGBufferRender(Shader& gbufferShader)
{
	glm::mat4 model = this->transform.MatrixWorld();
	gbufferShader.setMat4("model", model);
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

ObjectType Mesh::GetType()
{
	return ObjectType::MESH_OBJECT;
}
