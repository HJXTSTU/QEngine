#pragma once
#include "gl_transform.h"
#include "shader.h"
#include <memory>
#include <set>
#include "gl_render_texture.h"

using namespace std;

enum ObjectType {
	GROUP_OBJECT,
	MESH_OBJECT
};

class Object3D:public enable_shared_from_this<Object3D> {
public:
	Transform transform;
	vector<std::shared_ptr<Object3D>> children;
	string name;
public:
	Object3D();
	
	void AddChild(std::shared_ptr<Object3D> child);

	virtual void OnGBufferRender(Shader& gbufferShader) = 0;

	virtual void OnSurfaceRender() = 0;

	virtual void OnSurfaceRender(const RenderTexture &lightBuffer) = 0;

	virtual void OnSurfaceRender(const RenderTexture &lightBuffer, const RenderTexture &shadowmap) = 0;

	virtual void OnShadowmapRender(Shader& shader) = 0;

	virtual ObjectType GetType() = 0;

	shared_ptr<Object3D> AsPtr();

	template<typename T>
	shared_ptr<T> As() {
		return dynamic_pointer_cast<T>(shared_from_this());
	}
};