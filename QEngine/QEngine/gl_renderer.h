#pragma once
#include "gl_base_world.h"

class BaseRenderer;
typedef shared_ptr<BaseRenderer> BaseRendererPointer;

class BaseRenderer {
public:
	virtual void OnRender(BaseWorldPointer pWorld) = 0;
};