#include "loader_util.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "gl_model.h"
#include <string>
#include "util.h"

void processMesh(std::shared_ptr<AssetsManager> assets, ModelPointer model, std::string &directory,std::string &name, aiMesh *mesh, const aiScene *scene) {
	GeometryPointer pGeo = make_shared<Geometry>();
	pGeo->name = mesh->mName.C_Str();
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.UV.x = mesh->mTextureCoords[0][i].x;
			vertex.UV.y = mesh->mTextureCoords[0][i].y;

			vertex.Tangents.x = mesh->mTangents[i].x;
			vertex.Tangents.y = mesh->mTangents[i].y;
			vertex.Tangents.z = mesh->mTangents[i].z;
		}

		pGeo->PushVertex(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			pGeo->PushIndex(face.mIndices[j]);
		}
	}
	MapDesc mapDesc;
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString texPath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
			string path = texPath.C_Str();
			TexturePointer tex = std::make_shared<Texture>((directory+path).c_str());
			assets->AddTexture(name + "_" + "diffuse",tex->AsPtr());
			mapDesc.maps["DIFFUSE_MAP"] = tex;
		}
		if (material->GetTextureCount(aiTextureType_SPECULAR)) {
			aiString texPath;
			material->GetTexture(aiTextureType_SPECULAR, 0, &texPath);
			string path = texPath.C_Str();
			TexturePointer tex = std::make_shared<Texture>((directory + path).c_str());
			assets->AddTexture(name + "_" + "specular", tex->AsPtr());
			mapDesc.maps["SPECULAR_MAP"] = tex;
		}
		if (material->GetTextureCount(aiTextureType_HEIGHT)) {
			aiString texPath;
			material->GetTexture(aiTextureType_HEIGHT, 0, &texPath);
			string path = texPath.C_Str();
			TexturePointer tex = std::make_shared<Texture>((directory + path).c_str());
			assets->AddTexture(name + "_" + "normals", tex->AsPtr());
			mapDesc.maps["NORMAL_MAP"] = tex;
		}
	}

	model->PushGeometry(pGeo, mapDesc);
}

void processNode(std::shared_ptr<AssetsManager> assets, ModelPointer model, std::string &directory,std::string &name, aiNode *node, const aiScene *scene) {
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(assets, model, directory,name, mesh, scene);
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(assets, model, directory, name, node->mChildren[i], scene);
	}
}

ModelPointer loadModelFromFile(std::shared_ptr<AssetsManager> assets,std::string &path,std::string &name) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, 
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace| aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LogError(string(std::string("ERROR::ASSIMP::") + std::string(importer.GetErrorString())));
		return NULL;
	}

	string directory = path.substr(0, path.find_last_of('/'))+"/";
	
	ModelPointer pModel = make_shared<Model>();
	processNode(assets, pModel,directory,name, scene->mRootNode, scene);

	return pModel;
}

void loadModels(std::shared_ptr<AssetsManager> assets, CJsonObject models) {
	int size = models.GetArraySize();
	for (int i = 0; i < size; i++) {
		CJsonObject model = models[i];
		if (!model.HasKey("PATH")) {
			LogError("Miss model path.");
			continue;
		}
		
		if (!model.HasKey("NAME")) {
			LogError("Miss model name");
			continue;
		}

		std::string path = model.Get<std::string>("PATH");
		std::string name = model.Get<std::string>("NAME");
		
		ModelPointer pModel = loadModelFromFile(assets, path,name);
		assets->AddModel(name, pModel);
	}
}