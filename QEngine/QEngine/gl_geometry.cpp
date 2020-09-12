#include "gl_geometry.h"

void Geometry::PushVertex(Vertex &v) {
	this->vertices.push_back(v);
}

void Geometry::PushIndex(const unsigned int & index) {
	this->indices.push_back(index);
}

GeometryPointer Geometry::AsPtr()
{
	return shared_from_this();
}

void Geometry::caculateTangent(Vertex &p0, Vertex &p1, Vertex &p2) {
	glm::vec3 edge1 = glm::normalize(p1.Position - p0.Position);
	glm::vec3 edge2 = glm::normalize(p2.Position - p0.Position);
	glm::vec2 deltaUV1 = glm::normalize(p1.UV - p0.UV);
	glm::vec2 deltaUV2 = glm::normalize(p2.UV - p0.UV);

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	glm::vec3 tangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);

	glm::vec3 bitangent;
	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent = glm::normalize(bitangent);

	p2.Tangents = p1.Tangents = p0.Tangents = tangent;
	//p2.Bitangents = p1.Bitangents = p0.Bitangents = bitangent;
}

GeometryPointer Geometry::GeneratePlaneGeometry(float size, float texcoordSize) {
	float L = size / 2.0f;
	float R = -size / 2.0f;
	float F = size / 2.0f;
	float B = -size / 2.0f;
	GeometryPointer geo = std::make_shared<Geometry>();
	Vertex vertices[4];
	vertices[0].Position = glm::vec3(R, 0, F);
	vertices[0].UV = glm::vec2(0, 0);


	vertices[1].Position = glm::vec3(R, 0, B);
	vertices[1].UV = glm::vec2(0, texcoordSize);

	vertices[2].Position = glm::vec3(L, 0, B);
	vertices[2].UV = glm::vec2(texcoordSize, texcoordSize);

	vertices[3].Position = glm::vec3(L, 0, F);
	vertices[3].UV = glm::vec2(texcoordSize, 0);

	caculateTangent(vertices[0], vertices[1], vertices[2]);
	caculateTangent(vertices[0], vertices[2], vertices[3]);

	for (int i = 0; i < 4; i++) {
		vertices[i].Normal = glm::vec3(0, 0, 1);
	}

	for (int i = 0; i < 4; i++) {
		geo->vertices.push_back(vertices[i]);
	}

	geo->indices.push_back(0);
	geo->indices.push_back(1);
	geo->indices.push_back(2);
	geo->indices.push_back(0);
	geo->indices.push_back(2);
	geo->indices.push_back(3);
	return geo;
}

GeometryPointer Geometry::Clone() {
	GeometryPointer pGeo = std::shared_ptr<Geometry>();
	pGeo->vertices = this->vertices;
	pGeo->indices = this->indices;
	return pGeo;
}

