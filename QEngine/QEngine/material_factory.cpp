#include "material_factory.h"
#include "diffuse_material.h"


MaterialPointer MaterialFactory::CreateMaterial(const std::string &type) {
	if (type == "DIFFUSE") {
		return MaterialFactory::MakeMaterial<DiffuseMaterial>();
	}
}