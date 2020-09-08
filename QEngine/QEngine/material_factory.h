#pragma once
#include "gl_material.h"
class MaterialFactory{

private:
	template<typename MatType>
	static MaterialPointer MakeMaterial() {
		return MaterialPointer(new MatType());
	}
public:
	

	static MaterialPointer CreateMaterial(const std::string &type);
};