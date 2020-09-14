#include "gl_script_manager.h"

void ScriptsManager::AddScript(const string &name, BaseScriptPointer script) {
	if (objectToScripts.find(name) != objectToScripts.end()) {
		objectToScripts[name].push_back(script);
	}
	else {
		vector<BaseScriptPointer> scripts;
		scripts.push_back(script);
		objectToScripts[name] = scripts;
	}
	scripts.push_back(script);
}

void ScriptsManager::UpdateScripts(float deltaTime) {
	for (int i = 0; i < scripts.size(); i++) {
		scripts[i]->OnUpdate(deltaTime);
	}
}

void BaseScript::OnCreate(shared_ptr<Object3D> bindingObject) {
	gameObject = bindingObject;
}

BaseScriptPointer BaseScript::AsPtr() {
	return shared_from_this();
}