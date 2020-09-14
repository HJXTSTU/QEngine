#pragma once
#include <memory>
#include "gl_object3D.h"
#include "gl_group.h"
#include "gl_mesh.h"
#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include "util.h"

using namespace std;

class ObjectManager {
private:
	shared_ptr<Group> root;
	unordered_map<string, shared_ptr<Object3D>> nameToObject;
public:
	ObjectManager();

	shared_ptr<Object3D> FindObjectByName(const string &name);

	bool AddObject(const string &name, shared_ptr<Object3D> obj);

	bool AddObject(const string &parent, const string &name, shared_ptr<Object3D> obj);

	void UpdateMatrixWorld();

	const shared_ptr<Group> Root();
};
