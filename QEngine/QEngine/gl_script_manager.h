#pragma once
#include "gl_object3D.h"
#include <vector>
#include <unordered_map>

using namespace std;

class BaseScript;
typedef shared_ptr<BaseScript> BaseScriptPointer;
class ScriptsManager;

class ScriptsManager {
private:
	vector<BaseScriptPointer> scripts;
	unordered_map<string, vector<BaseScriptPointer>> objectToScripts;
public:
	void AddScript(const string &name, BaseScriptPointer script);

	void UpdateScripts(float deltaTime);
};

class BaseScript:public enable_shared_from_this<BaseScript> {
protected:
	shared_ptr<Object3D> gameObject;
public:
	virtual void OnCreate(shared_ptr<Object3D> bindingObject);

	virtual void OnUpdate(float deltaTime) = 0;

	BaseScriptPointer AsPtr();
};
