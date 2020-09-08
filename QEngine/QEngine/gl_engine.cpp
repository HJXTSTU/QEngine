#include "gl_engine.h"
std::shared_ptr<GLEngine> GLEngine::instance = NULL;
std::shared_ptr<GLEngine> GLEngine::Instance() {
	if (!instance) {
		instance = std::shared_ptr<GLEngine>(new GLEngine());
	}
	return instance;
}

void GLEngine::Run(std::shared_ptr<GLWindow> window, std::shared_ptr<Input> inputInstance, std::shared_ptr<ISceneHandler> scene) {
	if (scene) {
		scene->Awake();
		while (!window->ShouldClose()) {
			inputInstance->refreshKeyStatus(window);
			if (Input::getKeyDown(KeyCode::KEY_CODE_ESCAPE)) {
				window->CloseWindow();
				continue;
			}
			scene->Update(0.02f);
			scene->Render();
			window->EndRender();
		}

		scene->Destory();
	}
}