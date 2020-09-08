#ifndef GL_ENGINE
#define GL_ENGINE
#include "gl_window.h"
#include "input.h"
#include "gl_scene_handler.h"
class GLEngine {
private:
	static std::shared_ptr<GLEngine> instance;
public:
	static std::shared_ptr<GLEngine> Instance();
private:
	GLEngine() {

	}
public:
	void Run(std::shared_ptr<GLWindow> window, std::shared_ptr<Input> inputInstance, std::shared_ptr<ISceneHandler> scene);
};
#endif
