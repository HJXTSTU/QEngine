#include "gl_context.h"
//#include "input.h"
std::shared_ptr<GLWindow> Context::window;
Camera Context::mainCamera;

Context::Context()
{
	m_inputModel = Input::Instance();
}

void Context::BeginFrame()
{
	if(Context::window)
		this->m_inputModel->refreshKeyStatus(Context::window);
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
