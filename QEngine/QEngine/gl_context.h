#pragma once
#include <glad/glad.h>
#include <memory>
#include <iostream>
#include <fstream>
#include "input.h"
#include "gl_camera.h"
#include "gl_uniform_buffer.h"
#include "gl_assets.h"
#include "gl_base_world.h"
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

	static BaseWorldPointer world;
public:
	static ContextPointer CreateContext();

	static void SetWorld(BaseWorldPointer world);

private:
	void InitWorld();
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

