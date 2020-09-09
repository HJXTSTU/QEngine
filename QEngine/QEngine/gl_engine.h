#ifndef GL_ENGINE
#define GL_ENGINE
#include "gl_window.h"
#include "gl_context.h"
class GLEngine {
private:
	static std::shared_ptr<GLEngine> instance;
public:
	static std::shared_ptr<GLEngine> Instance();
private:
	GLEngine() {

	}
public:
	void Run(Context &context);
};
#endif
