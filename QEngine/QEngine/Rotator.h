#pragma once
#include "gl_script_manager.h"
class Rotator :public BaseScript {
private:
	shared_ptr<Object3D> cubes;
public:
	virtual void OnCreate(shared_ptr<Object3D> bindingObject) {
		BaseScript::OnCreate(bindingObject);
		//	TO DO:: Other
		cubes = bindingObject;

	}

	virtual void OnUpdate(float deltaTime) {
		if(this->cubes)
			this->cubes->transform.Rotate(glm::radians(10.0f*deltaTime),glm::vec3(0,1,0));
	}
};