#pragma once
#include "gl_geometry.h"
#include <unordered_map>
#include <vector>
#include "gl_mesh.h"
#include "gl_group.h"
#include "gl_texture.h"
#include "gl_material.h"
#include <memory>
using namespace std;
struct MapDesc{
	
	//	KEY:NAME
	unordered_map<string, TexturePointer> maps;
};

class Model;

typedef shared_ptr<Model> ModelPointer;

class Model:public enable_shared_from_this<Model> {
private:
	GLuint m_uiMeshCount;
	vector<GeometryPointer> m_arrGeometries;
	vector<MapDesc>  m_arrMapDescs;
public:
	Model();

	void PushGeometry(GeometryPointer geometry, MapDesc materialDesc);
	
	std::shared_ptr<Object3D> Instantiate(MaterialPointer prototype_mat);

	ModelPointer AsPtr();
};

