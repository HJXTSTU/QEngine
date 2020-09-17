#pragma once
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

namespace QEngine {
	struct EngineShell;
	typedef std::shared_ptr<EngineShell> EngineShellPointer;
	
	struct EngineShell {
	private:
		EngineShell();

		bool InitGLAD();

		bool CreateEngine();

		bool CreateGLWindow();

		bool LoadAssets();

	private:
		static EngineShellPointer instance;
	public:
		static EngineShellPointer Instance();

	public:
		bool LaunchQEngine();

		void Run();
	public:
		std::shared_ptr<GLEngine> gEngine;
		std::shared_ptr<GLWindow> gWindow;
		std::shared_ptr<AssetsManager> gAssets;
		ContextPointer gContext;

		
	};
}