#pragma once
#include "gl_transform.h"
#include "shader.h"
#include <memory>
#include <set>
enum ObjectType {
	GROUP_OBJECT,
	MESH_OBJECT
};

class Object3D {
public:
	Transform transform;
	std::vector<std::shared_ptr<Object3D>> children;
public:
	Object3D();
	
	void AddChild(std::shared_ptr<Object3D> child);

	virtual void OnGBufferRender(Shader& gbufferShader) = 0;

	virtual void OnSurfaceRender() = 0;

	virtual ObjectType GetType() = 0;
};