#pragma once
#include <glad/glad.h>
#include "gl_context_interfaces.h"
#include <memory>
#include <iostream>
#include <fstream>
#include "input.h"
#include "gl_camera.h"

using namespace std;
class Context;

class Context {
private:

public:
	static std::shared_ptr<GLWindow> window;
	
	static Camera mainCamera;
private:
	std::shared_ptr<Input> m_inputModel;
public:
	Context();



	void BeginFrame();
	void PreUpdate();

	void Update();

	void LateUpdate();

	void Render();

	void EndFrame();
};
