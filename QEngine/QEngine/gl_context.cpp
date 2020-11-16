#include "gl_context.h"
#include "CJsonObject.hpp"
#include "gl_util.h"
#include "direction_light.h"
#include "point_light.h"
using namespace neb;
//#include "input.h"

UniformBufferCameraPointer Context::pUniformBufferCamera;

std::shared_ptr<GLWindow> Context::window;

std::shared_ptr<AssetsManager> Context::assets;

Camera Context::mainCamera;

BaseWorldPointer Context::world;

BaseRendererPointer Context::renderer;


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

void Context::InitWorld(const string &configFilePath) {
	CJsonObject worldObjectsConfig(ReadJsonFile(configFilePath));
	if (worldObjectsConfig.HasKey("OBJECTS")) {
		CJsonObject objects = worldObjectsConfig.Get<CJsonObject>("OBJECTS");
		int size = objects.GetArraySize();
		for (int i = 0; i < size; i++) {
			CJsonObject object = objects[i];
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

			if (object.HasKey("SCALE")) {
				CJsonObject scaleArray = object.Get<CJsonObject>("SCALE");
				int scale_size = scaleArray.GetArraySize();
				float scale[3] = { 1,1,1 };
				for (int j = 0; j < scale_size; j++) {
					scaleArray.Get(j, scale[j]);
				}
				obj->transform.Scale(glm::vec3(scale[0],scale[1],scale[2]), false);
			}
			if (!Context::world->AddObject(parent_name, name, obj)) {
				LogError("Add object faile.");
			}
		}
	}
	else {
		LogError("World config doesn't contain key---(OBJECTS)");
	}

	if (worldObjectsConfig.HasKey("LIGHTS")) {
		CJsonObject lights = worldObjectsConfig.Get<CJsonObject>("LIGHTS");
		int size = lights.GetArraySize();
		for (int i = 0; i < size; i++) {
			CJsonObject light = lights[i];
			string type = light.Get<string>("TYPE");
			if (type == "DIRECTION") {
				CJsonObject params = light.Get<CJsonObject>("PARAMS");
				CJsonObject lightColor = params.Get<CJsonObject>("LIGHT_COLOR");
				CJsonObject lightDirection = params.Get<CJsonObject>("LIGHT_DIRECTION");
				float r = lightColor.Get<float>(0);
				float g = lightColor.Get<float>(1);
				float b = lightColor.Get<float>(2);
				glm::vec3 color(r, g, b);
				
				float x = lightDirection.Get<float>(0);
				float y = lightDirection.Get<float>(1);
				float z = lightDirection.Get<float>(2);
				glm::vec3 direction(x, y, z);

				float maxBias = 0.3f;
				float minBias = 0.05f;
				float normalBias = 0.3f;
				if (params.HasKey("MAX_BIAS")) {
					maxBias = params.Get<float>("MAX_BIAS");
				}
				if (params.HasKey("MIN_BIAS")) {
					minBias = params.Get<float>("MIN_BIAS");
				}
				if (params.HasKey("NORMAL_BIAS")) {
					normalBias = params.Get<float>("NORMAL_BIAS");
				}

				LightPointer light = DirectionLight::Create(color,direction);
				light->SetMaxBias(maxBias);
				light->SetMinBias(minBias);
				light->SetNormalBias(normalBias);
				
				world->AddLight(light);
			}
			else if (type == "POINT") {
				CJsonObject params = light.Get<CJsonObject>("PARAMS");
				CJsonObject lightColor = params.Get<CJsonObject>("LIGHT_COLOR");
				CJsonObject lightPosition = params.Get<CJsonObject>("POSITION");

				float r = lightColor.Get<float>(0);
				float g = lightColor.Get<float>(1);
				float b = lightColor.Get<float>(2);
				glm::vec3 color(r, g, b);

				float x = lightPosition.Get<float>(0);
				float y = lightPosition.Get<float>(1);
				float z = lightPosition.Get<float>(2);
				glm::vec3 position(x, y, z);
				
				float constant = 1.0f;
				float linear = 0.7f;
				float quadratic = 1.8f;
				if (params.HasKey("CONSTANT")) {
					constant = params.Get<float>("CONSTANT");
				}
				if (params.HasKey("LINEAR")) {
					linear = params.Get<float>("LINEAR");
				}
				if (params.HasKey("QUADRATIC")) {
					quadratic = params.Get<float>("QUADRATIC");
				}

				glm::vec3 attenuationParams(constant, linear, quadratic);

				
				float maxBias = 0.3f;
				float minBias = 0.05f;
				float normalBias = 0.3f;
				if (params.HasKey("MAX_BIAS")) {
					maxBias = params.Get<float>("MAX_BIAS");
				}
				if (params.HasKey("MIN_BIAS")) {
					minBias = params.Get<float>("MIN_BIAS");
				}
				if (params.HasKey("NORMAL_BIAS")) {
					normalBias = params.Get<float>("NORMAL_BIAS");
				}

				LightPointer light = PointLight::Create(color, position, attenuationParams);
				light->SetMaxBias(maxBias);
				light->SetMinBias(minBias);
				light->SetNormalBias(normalBias);

				world->AddLight(light);
			}
		}
	}
}

void Context::SetWorld(BaseWorldPointer world)
{
	Context::world = world;
}

void Context::SetWorldAndInit(BaseWorldPointer world, const string & configFilePath)
{
	Context::SetWorld(world);
	InitWorld(configFilePath);
}

void Context::SetRenderer(BaseRendererPointer renderer)
{
	Context::renderer = renderer;
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
	if (Context::world) {
		Context::world->UpdateWorld(0.02f);
	}
		
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
	if (Context::renderer) {
		Context::renderer->OnRender(Context::world);
	}
	else {
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void Context::EndFrame()
{
	
}

ContextPointer Context::AsPtr()
{
	return shared_from_this();
}
