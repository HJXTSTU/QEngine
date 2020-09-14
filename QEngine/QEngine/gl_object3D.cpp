#include "gl_object3D.h"

Object3D::Object3D()
{
}

void Object3D::AddChild(std::shared_ptr<Object3D> child)
{
	this->transform.Add(&child->transform);
	this->children.push_back(child);
}

shared_ptr<Object3D> Object3D::AsPtr() {
	return shared_from_this();
}