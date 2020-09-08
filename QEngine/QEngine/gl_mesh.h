#pragma once
#include "gl_object3D.h"
#include "gl_material.h"
#include "gl_geometry_vertex_array.h"
class Mesh :public Object3D {
private:
	GeometryPointer pGeometry;
	MaterialPointer pMaterial;
	GLGeometryVertexArray vertexArray;
public:
	Mesh(GeometryPointer geometry, MaterialPointer material);

	virtual void OnGBufferRender(Shader &gbufferShader);

	virtual void OnSurfaceRender();

	virtual ObjectType GetType();
};