#pragma once
#include "gl_script_manager.h"
class Rotator :public BaseScript {
public:
	virtual void OnCreate(shared_ptr<Object3D> bindingObject) {
		BaseScript::OnCreate(bindingObject);
		//	TO DO:: Other
	}

	virtual void OnUpdate(float deltaTime) {
		this->gameObject->transform.Rotate(glm::radians(3.0f*deltaTime),glm::vec3(0,1,0));
	}
};