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
#include "gl_object_manager.h"
#include "gl_script_manager.h"
#include "light.h"

using namespace std;
class BaseWorld;
typedef shared_ptr<BaseWorld> BaseWorldPointer;

class BaseWorld :public enable_shared_from_this<BaseWorld> {
private:
	ObjectManager m_objectManager;
	ScriptsManager m_scriptManager;
	vector<LightPointer>  m_lights;
public:

	shared_ptr<Object3D> FindObjectByName(const string &name);

	bool AddObject(const string &name, shared_ptr<Object3D> obj);

	bool AddObject(const string &parent, const string &name, shared_ptr<Object3D> obj);

	void BindScriptTo(BaseScriptPointer script, const string &name);

	void UpdateWorld(float deltaTime);

	vector<LightPointer> &GetLights() { return m_lights; }

	void AddLight(LightPointer light) { m_lights.push_back(light); }

	const shared_ptr<Group> Root();

	BaseWorldPointer AsPtr();
};