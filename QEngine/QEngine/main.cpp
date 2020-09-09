#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl_engine.h"
#include "constant.h"
#include "input.h"
#include <memory>
#include <iostream>
#include <fstream>
#include "gl_assets.h"
#include "CJsonObject.hpp"
#include "loader_util.h"
#include "gl_context.h"
#include "gl_engine.h"
#include <iostream>
using namespace std;

std::shared_ptr<GLEngine> gEngine;
std::shared_ptr<GLWindow> gWindow;
Context gContext;

bool InitGLAD() {
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	glEnable(GL_MULTISAMPLE);
	return true;
}

bool CreateEngine() {
	gEngine = GLEngine::Instance();
	if (gEngine == NULL) {
		cout << "Create Engine Faile." << endl;
		return false;
	}
	return true;
}

bool CreateGLWindow() {
	WindowBuilder builder;
	gWindow = builder.BuildDefaultWindow();
	if (gWindow == NULL) {
		cout << "Build window faile." << endl;
		return false;
	}
	return true;
}


int main() {

	//initializeAssets();

	if (!CreateEngine()) {
		return 1;
	}

	if (!CreateGLWindow()) {
		return 1;
	}

	if (!InitGLAD()) {
		return 1;
	}


	Context::window = gWindow;

	if (gEngine != NULL) {
		gEngine->Run(gContext);
	}
	gWindow->Destory();
	return 0;
}