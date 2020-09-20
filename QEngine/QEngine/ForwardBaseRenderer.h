#pragma once
#include "gl_renderer.h"
class ForwardBaseRenderer :public BaseRenderer {
public:
	virtual void OnRender(BaseWorldPointer pWorld) {
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		pWorld->Root()->OnSurfaceRender();
	}
};