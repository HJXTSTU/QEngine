#include "gl_base_world.h"

shared_ptr<Object3D> BaseWorld::FindObjectByName(const string & name)
{
	return m_objectManager.FindObjectByName(name);
}

bool BaseWorld::AddObject(const string & name, shared_ptr<Object3D> obj)
{
	return m_objectManager.AddObject(name,obj);
}

bool BaseWorld::AddObject(const string & parent, const string & name, shared_ptr<Object3D> obj)
{
	return m_objectManager.AddObject(parent,name,obj);
}

void BaseWorld::BindScriptTo(BaseScriptPointer script, const string & name)
{
	m_scriptManager.AddScript(name,script);
	shared_ptr<Object3D> obj = m_objectManager.FindObjectByName(name);
	if (obj) {
		script->OnCreate(obj);
	}
	else {
		LogError(string("Can't not find ") + name);
	}
	
}

void BaseWorld::UpdateWorld(float deltaTime)
{
	m_scriptManager.UpdateScripts(deltaTime);
	m_objectManager.UpdateMatrixWorld();
}

const shared_ptr<Group> BaseWorld::Root()
{
	return m_objectManager.Root();
}

BaseWorldPointer BaseWorld::AsPtr()
{
	return shared_from_this();
}
