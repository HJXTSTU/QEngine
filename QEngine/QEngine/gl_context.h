#pragma once
#include <glad/glad.h>
#include "gl_context_interfaces.h"
#include <memory>
#include <iostream>
#include <fstream>
#include "input.h"
#include "gl_camera.h"
#include "gl_uniform_buffer.h"

using namespace std;

typedef GLUniformBuffer<GLOBAL_UNIFORM_CAMERA_MATRIES_INDEX, GLOBAL_UNIFORM_CAMERA_MATRIES_SIZE> UniformBufferCamera;

typedef std::shared_ptr<UniformBufferCamera> UniformBufferCameraPointer;

class Context {
private:
	static UniformBufferCameraPointer pUniformBufferCamera;

	static void CursorScrollCallback(GLFWwindow* window, double xpos, double ypos);

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
public:
	static std::shared_ptr<GLWindow> window;
	
	static Camera mainCamera;
public:
	Context();

	void BeginFrame();
	void PreUpdate();

	void Update();

	void LateUpdate();

	void Render();

	void EndFrame();
};

