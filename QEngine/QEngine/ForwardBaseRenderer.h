#pragma once
#include "gl_renderer.h"
class ForwardBaseRenderer :public BaseRenderer {
public:
	virtual void OnRender(BaseWorldPointer pWorld);
};