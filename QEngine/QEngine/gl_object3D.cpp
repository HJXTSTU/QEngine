#include "gl_object3D.h"

Object3D::Object3D()
{
}

void Object3D::AddChild(std::shared_ptr<Object3D> child)
{
	this->transform.Add(&child->transform);
	this->children.push_back(child);
}
