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
