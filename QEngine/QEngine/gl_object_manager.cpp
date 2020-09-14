#include "gl_object_manager.h"

ObjectManager::ObjectManager() {
	root = make_shared<Group>();
	nameToObject["__ROOT__"] = root;
}

shared_ptr<Object3D> ObjectManager::FindObjectByName(const string &name) {
	if (nameToObject.find(name) == nameToObject.end()) {
		LogError("Object name does not exist.");
		return NULL;
	}
	return nameToObject[name];
}

bool ObjectManager::AddObject(const string &name, shared_ptr<Object3D> obj) {
	return AddObject("__ROOT__", name, obj);
}

bool ObjectManager::AddObject(const string &parent, const string &name, shared_ptr<Object3D> obj) {
	if (nameToObject.find(parent) == nameToObject.end()) {
		LogError("Parent object name does not exist.");
		return false;
	}
	int i = 1;
	string key = name;
	if (nameToObject.find(name) == nameToObject.end()) {
		string nextName;
		do {
			string nextName = name + +"_" + ItoStr(i++);
		} while (nameToObject.find(nextName) == nameToObject.end());
		key = nextName;
	}
	nameToObject[key] = obj;
	nameToObject[parent]->AddChild(obj);
	return true;
}

void ObjectManager::UpdateMatrixWorld()
{
	root->transform.UpdateMatrixWorld(true);
}

const shared_ptr<Group> ObjectManager::Root()
{
	return root;
}
