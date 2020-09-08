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
using namespace std;
using namespace neb;

std::shared_ptr<GLWindow> gWindow;
std::shared_ptr<Input> gInputModel;
std::shared_ptr<GLEngine> gEngine;
std::shared_ptr<AssetsManager> gAssets;

GLboolean initialization() {
	gEngine = GLEngine::Instance();
	if (gEngine == NULL) {
		cout << "Create Engine Faile." << endl;
		return GL_FALSE;
	}

	WindowBuilder builder;
	gWindow = builder.BuildDefaultWindow();
	if (gWindow == NULL) {
		cout << "Build window faile." << endl;
		return GL_FALSE;
	}

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return GL_FALSE;
	}


	glEnable(GL_MULTISAMPLE);

	gInputModel = Input::Instance();
}





void initializeAssets() {
	gAssets = make_shared<AssetsManager>();
	ifstream fin("./loading_config.json");
	fin.seekg(0, std::ios::end);
	int length = fin.tellg();
	fin.seekg(0, std::ios::beg);
	char *config = new char[length + 1];
	fin.read(config, length);
	fin.close();

	CJsonObject configJsonObj(config);
	if (configJsonObj.HasKey("TEXTURES")) {
		loadConfigTextures(gAssets, configJsonObj.Get<CJsonObject>("TEXTURES"));
	}

	if (configJsonObj.HasKey("MATERIALS")) {
		createMaterials(gAssets, configJsonObj.Get<CJsonObject>("MATERIALS"));
	}
}



int main() {

	if (!initialization()) {
		cout << "Initialization System Faile." << endl;
		return 1;
	}

	initializeAssets();

	if (gEngine != NULL) {
		gEngine->Run(gWindow, gInputModel, NULL);
	}
	gWindow->Destory();
}