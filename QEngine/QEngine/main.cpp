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
std::shared_ptr<AssetsManager> gAssets;
ContextPointer gContext;

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

bool LoadAssets() {
	gAssets = make_shared<AssetsManager>();

	ifstream fin("loading_config.json");
	fin.seekg(0,fin.end);
	unsigned int length = fin.tellg();
	fin.seekg(0, fin.beg);
	char *json = new char[length +1];
	fin.read(json, length);

	CJsonObject config(json);
	
	if (config.HasKey("TEXTURES")) {
		CJsonObject textures = config.Get<CJsonObject>("TEXTURES");
		loadConfigTextures(gAssets, textures);
	}

	if (config.HasKey("MATERIALS")) {
		CJsonObject materials = config.Get<CJsonObject>("MATERIALS");
		createMaterials(gAssets, materials);
	}

	if (config.HasKey("MODELS")) {
		CJsonObject models = config.Get<CJsonObject>("MODELS");
		loadModels(gAssets, models);
	}
	return true;
}


int main() {

	if (!CreateEngine()) {
		return 1;
	}

	if (CreateGLWindow()) {
		Context::window = gWindow;
	}
	else {
		return 1;
	}

	if (!InitGLAD()) {
		return 1;
	}

	if (LoadAssets()) {
		Context::assets = gAssets;
	}
	else {
		return 1;
	}

	gContext = Context::CreateContext();

	if (gEngine != NULL) {
		gEngine->Run(gContext);
	}
	gWindow->Destory();
	return 0;
}