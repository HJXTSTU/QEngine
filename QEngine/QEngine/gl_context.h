#pragma once
#include <glad/glad.h>
#include "gl_context_interfaces.h"
#include <memory>
#include <iostream>
#include <fstream>
#include "input.h"
#include "gl_camera.h"
#include "gl_uniform_buffer.h"
#include "gl_assets.h"

#include "gl_mesh.h"

using namespace std;

typedef GLUniformBuffer<GLOBAL_UNIFORM_CAMERA_MATRIES_INDEX, GLOBAL_UNIFORM_CAMERA_MATRIES_SIZE> UniformBufferCamera;

typedef std::shared_ptr<UniformBufferCamera> UniformBufferCameraPointer;
class Context;
typedef std::shared_ptr<Context> ContextPointer;
class Context:public std::enable_shared_from_this<Context> {
private:
	static void CursorScrollCallback(GLFWwindow* window, double xpos, double ypos);

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
private:
	//	Uniform Buffers
	static UniformBufferCameraPointer pUniformBufferCamera;
public:
	static std::shared_ptr<GLWindow> window;

	static std::shared_ptr<AssetsManager> assets;
	
	static Camera mainCamera;
public:
	static ContextPointer CreateContext();
private:
	std::shared_ptr<Object3D> nanosuit;

	std::shared_ptr<Mesh> plane;
public:

	Context();

	void BeginFrame();

	void PreUpdate();

	void Update();

	void LateUpdate();

	void Render();

	void EndFrame();

	ContextPointer AsPtr();
};

