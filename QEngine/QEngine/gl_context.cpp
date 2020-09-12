#include "gl_context.h"
//#include "input.h"

UniformBufferCameraPointer Context::pUniformBufferCamera;

std::shared_ptr<GLWindow> Context::window;

std::shared_ptr<AssetsManager> Context::assets;

Camera Context::mainCamera;


void Context::CursorScrollCallback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	static float lastX = 0.0f;
	static float lastY = 0.0f;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	Context::mainCamera.ProcessMouseMovement(xoffset, yoffset);
}

void Context::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Context::mainCamera.ProcessMouseScroll(yoffset);
}

ContextPointer Context::CreateContext() {
	Context::pUniformBufferCamera = make_shared<UniformBufferCamera>();
	assert(Context::window);
	Context::window->SetCursorPosCallback(Context::CursorScrollCallback);
	Context::window->SetScrollCallback(Context::ScrollCallback);
	return make_shared<Context>();
}

Context::Context()
{
	MaterialPointer mat = Context::assets->GetMaterial("diffuse");
	nanosuit = Context::assets->GetModel("nanosuit")->Instantiate(mat);
	plane = std::make_shared<Mesh>(Geometry::GeneratePlaneGeometry(), mat->Clone());
}

void Context::BeginFrame()
{
	if (Context::window) {
		InputUpdator::instance.refreshKeyStatus(Context::window);
	}

	Context::pUniformBufferCamera->FillBufferData<mat4>(0, Context::mainCamera.GetProjectionMatrix(SRC_WIDTH, SRC_HEIGHT));
	Context::pUniformBufferCamera->FillBufferData<mat4>(64, Context::mainCamera.GetViewMatrix());
	Context::pUniformBufferCamera->FillBufferData<vec3>(128, Context::mainCamera.Position);
}

void Context::PreUpdate()
{
}

void Context::Update()
{
	//nanosuit->transform.Scale(glm::vec3(0.1f, 0.1f, 0.1f),false);
	nanosuit->transform.UpdateMatrixWorld(true);
	plane->transform.UpdateMatrixWorld(true);
}

void Context::LateUpdate()
{

}

void Context::Render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	nanosuit->OnSurfaceRender();
	plane->OnSurfaceRender();
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
	Context::mainCamera.UpdateCameraVectorForce();
}

ContextPointer Context::AsPtr()
{
	return shared_from_this();
}
