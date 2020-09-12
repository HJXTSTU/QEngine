#include "gl_model.h"
Model::Model() {
	m_uiMeshCount = 0;
}

void Model::PushGeometry(GeometryPointer geometry, MapDesc materialDesc) {
	m_arrGeometries.push_back(geometry);
	m_arrMapDescs.push_back(materialDesc);
	m_uiMeshCount++;
}

std::shared_ptr<Object3D> Model::Instantiate(MaterialPointer prototype_mat) {
	std::shared_ptr<Group> group = std::make_shared<Group>();
	for (GLuint i = 0; i < m_uiMeshCount; i++) {
		GeometryPointer pGeo = m_arrGeometries[i];
		MaterialPointer pMat = prototype_mat->Clone();
		MapDesc mapDesc = m_arrMapDescs[i];
		for (auto it = mapDesc.maps.begin(); it != mapDesc.maps.end(); it++) {
			pMat->SetTexture(it->first, it->second);
		}
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(pGeo, pMat);
		group->AddChild(mesh);
	}
	return group;
}

ModelPointer Model::AsPtr() {
	return shared_from_this();
}