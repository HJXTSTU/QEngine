#include "gl_context.h"
#include "CJsonObject.hpp"
#include "gl_util.h"
using namespace neb;
//#include "input.h"

UniformBufferCameraPointer Context::pUniformBufferCamera;

std::shared_ptr<GLWindow> Context::window;

std::shared_ptr<AssetsManager> Context::assets;

Camera Context::mainCamera;

BaseWorldPointer Context::world;


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

void Context::InitWorld() {
	CJsonObject worldObjectsConfig(ReadJsonFile("world_config.json"));
	if (worldObjectsConfig.HasKey("OBJECTS")) {
		int size = worldObjectsConfig.GetArraySize();
		for (int i = 0; i < size; i++) {
			CJsonObject object = worldObjectsConfig[i];
			if (!object.HasKey("NAME")) {
				LogError("World config doesn't contain key---(NAME)");
				continue;
			}

			if (!object.HasKey("FROM_MODEL")) {
				LogError("World config doesn't contain key---(FROM_MODEL)");
				continue;
			}

			if (!object.HasKey("MATERIAL")) {
				LogError("World config doesn't contain key---(MATERIAL)");
				continue;
			}

			if (!object.HasKey("PARENT")) {
				LogError("World config doesn't contain key---(PARENT)");
				continue;
			}

			string name = object.Get<string>("NAME");
			string model_name = object.Get<string>("FROM_MODEL");
			string material_name = object.Get<string>("MATERIAL");
			string parent_name = object.Get<string>("PARENT");
			
			ModelPointer model = Context::assets->GetModel(model_name);
			if (!model) {
				char msg[512];
				sprintf(msg, "Model(%s) doesn't exist.", model_name);
				LogError(string(msg));
				continue;
			}
			MaterialPointer mat = Context::assets->GetMaterial(material_name);
			if (!mat) {
				char msg[512];
				sprintf(msg, "Material(%s) doesn't exist.", material_name);
				LogError(string(msg));
				continue;
			}

			shared_ptr<Object3D> obj = model->Instantiate(mat);
			obj->name = name;
			if (!Context::world->AddObject(parent_name, name, obj)) {
				LogError("Add object faile.");
			}
		}
	}
	else {
		LogError("World config doesn't contain key---(OBJECTS)");
	}
}

void Context::SetWorld(BaseWorldPointer world)
{
	Context::world = world;
}

Context::Context()
{
}

void Context::BeginFrame()
{
	
}

void Context::PreUpdate()
{
	if (Context::window) {
		InputUpdator::instance.refreshKeyStatus(Context::window);
	}
}

void Context::Update()
{
	Context::world->UpdateWorld(0.02f);
}

void Context::LateUpdate()
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

	Context::pUniformBufferCamera->FillBufferData<mat4>(0, Context::mainCamera.GetProjectionMatrix(SRC_WIDTH, SRC_HEIGHT));
	Context::pUniformBufferCamera->FillBufferData<mat4>(64, Context::mainCamera.GetViewMatrix());
	Context::pUniformBufferCamera->FillBufferData<vec3>(128, Context::mainCamera.Position);
}

void Context::Render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Context::EndFrame()
{
	
}

ContextPointer Context::AsPtr()
{
	return shared_from_this();
}
