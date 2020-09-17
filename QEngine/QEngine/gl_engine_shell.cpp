#include "gl_engine_shell.h"
using namespace QEngine;

EngineShell::EngineShell() {

}

bool EngineShell::InitGLAD() {
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

bool EngineShell::CreateEngine() {
	gEngine = GLEngine::Instance();
	if (gEngine == NULL) {
		cout << "Create Engine Faile." << endl;
		return false;
	}
	return true;
}

bool EngineShell::CreateGLWindow() {
	WindowBuilder builder;
	gWindow = builder.BuildDefaultWindow();
	if (gWindow == NULL) {
		cout << "Build window faile." << endl;
		return false;
	}
	return true;
}

bool EngineShell::LoadAssets() {
	gAssets = make_shared<AssetsManager>();

	ifstream fin("loading_config.json");
	fin.seekg(0, fin.end);
	unsigned int length = fin.tellg();
	fin.seekg(0, fin.beg);
	char *json = new char[length + 1];
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

bool EngineShell::LaunchQEngine() {
	if (!CreateEngine()) {
		LogError("Create Engine Faile.");
		return false;
	}

	if (CreateGLWindow()) {
		Context::window = gWindow;
	}
	else {
		LogError("Create Window Faile.");
		return false;
	}

	if (!InitGLAD()) {
		LogError("Init GLAD Faile.");
		return false;
	}

	if (LoadAssets()) {
		Context::assets = gAssets;
	}
	else {
		LogError("Load assets Faile.");
		return false;
	}

	gContext = Context::CreateContext();
	return true;
}

void QEngine::EngineShell::Run()
{
	if (gEngine != NULL) {
		gEngine->Run(gContext);
	}
	gWindow->Destory();
}

EngineShellPointer EngineShell::instance;
EngineShellPointer EngineShell::Instance()
{
	if (EngineShell::instance == NULL) {
		EngineShell::instance = EngineShellPointer(new EngineShell());
	}
	return instance;
}
