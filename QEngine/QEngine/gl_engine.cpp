#include "gl_engine.h"
std::shared_ptr<GLEngine> GLEngine::instance = NULL;
std::shared_ptr<GLEngine> GLEngine::Instance() {
	if (!instance) {
		instance = std::shared_ptr<GLEngine>(new GLEngine());
	}
	return instance;
}

GLEngine::GLEngine() {

}

void GLEngine::Run(ContextPointer context) {
	std::shared_ptr<GLWindow> window = Context::window;
	glEnable(GL_MULTISAMPLE);
	while (!window->ShouldClose()) {
		context->BeginFrame();
		context->PreUpdate();
		context->Update();
		context->LateUpdate();
		context->Render();
		context->EndFrame();
		window->EndRender();
	}
}