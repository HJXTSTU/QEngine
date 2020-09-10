#include "gl_context.h"
//#include "input.h"

UniformBufferCameraPointer Context::pUniformBufferCamera;

std::shared_ptr<GLWindow> Context::window;

Camera Context::mainCamera;

Context::Context()
{
	pUniformBufferCamera = make_shared<UniformBufferCamera>();
}

void Context::BeginFrame()
{
	if (Context::window)
		InputUpdator::instance.refreshKeyStatus(Context::window);
	Context::pUniformBufferCamera->RefleshBufferData<mat4>(0, Context::mainCamera.GetProjectionMatrix(SRC_WIDTH, SRC_HEIGHT));
	Context::pUniformBufferCamera->RefleshBufferData<mat4>(64, Context::mainCamera.GetViewMatrix());
	Context::pUniformBufferCamera->RefleshBufferData<vec3>(128, Context::mainCamera.Position);


}

void Context::PreUpdate()
{
}

void Context::Update()
{
}

void Context::LateUpdate()
{

}

void Context::Render()
{
}

void Context::EndFrame()
{
	if (Input::getKeyDown(KEY_CODE_ESCAPE)) {
		Context::window->CloseWindow();
		return;
	}

	if (Input::getKey(KEY_CODE_W)) {
		Context::mainCamera.ProcessKeyboard(Camera_Movement::FORWARD, 0.02f);
	}
	if (Input::getKey(KEY_CODE_S)) {
		Context::mainCamera.ProcessKeyboard(Camera_Movement::BACKWARD, 0.02f);
	}
	if (Input::getKey(KEY_CODE_A)) {
		Context::mainCamera.ProcessKeyboard(Camera_Movement::LEFT, 0.02f);
	}
	if (Input::getKey(KEY_CODE_D)) {
		Context::mainCamera.ProcessKeyboard(Camera_Movement::RIGHT, 0.02f);
	}
}
