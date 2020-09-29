#pragma once
#include "gl_script_manager.h"
class Rotator :public BaseScript {
private:
	shared_ptr<Object3D> dragon;
public:
	virtual void OnCreate(shared_ptr<Object3D> bindingObject) {
		BaseScript::OnCreate(bindingObject);
		//	TO DO:: Other
		for (int i = 0; i < bindingObject->children.size(); i++) {
			if (bindingObject->children[i]->name == "Dragon") {
				this->dragon = bindingObject->children[i];
				break;
			}
		}

	}

	virtual void OnUpdate(float deltaTime) {
		/*if(this->dragon)
			this->dragon->transform.Rotate(glm::radians(45.0f*deltaTime),glm::vec3(0,1,0));*/
	}
};