#pragma once
#include "gl_script_manager.h"
class Rotator :public BaseScript {
public:
	virtual void OnCreate(shared_ptr<Object3D> bindingObject) {
		BaseScript::OnCreate(bindingObject);
		//	TO DO:: Other
		this->gameObject->transform.Scale(vec3(0.1f, 0.1f, 0.1f));

	}

	virtual void OnUpdate(float deltaTime) {
		this->gameObject->transform.Rotate(glm::radians(45.0f*deltaTime),glm::vec3(0,1,0));
	}
};