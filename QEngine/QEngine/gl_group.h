#pragma once
#include "gl_object3D.h"

class Group :public Object3D {
public:
	Group();

	virtual void OnGBufferRender(Shader &gbufferShader);

	virtual void OnSurfaceRender();

	virtual ObjectType GetType();
};