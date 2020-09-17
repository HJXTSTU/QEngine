#include "loader_util.h"
#include "material_factory.h"
#include "util.h"

static void setupMaterial(const std::string &type, MaterialPointer material, CJsonObject &params, std::shared_ptr<AssetsManager> assets) {
	if (params.HasKey("MAPS")) {
		CJsonObject maps = params.Get<CJsonObject>("MAPS");
		int size = maps.GetArraySize();
		for (int i = 0; i < size; i++) {
			CJsonObject texDesc = maps[i];
			string key = texDesc.Get<string>("KEY");
			string value = texDesc.Get<string>("VALUE");
			TexturePointer tex = assets->GetTexture(value);
			material->SetTexture(key, tex);
		}
		
	}
}

void createMaterials(std::shared_ptr<AssetsManager> assets, CJsonObject materials) {
	int size = materials.GetArraySize();
	for (int i = 0; i < size; i++) {
		CJsonObject materialDesc = materials[i];
		if (!materialDesc.HasKey("NAME")) {
			LogError("Miss material name.");
			return;
		}
		std::string name = materialDesc.Get<std::string>("NAME");

		if (!materialDesc.HasKey("TYPE")) {
			LogError("Miss material type.");
			return;
		}

		std::string type = materialDesc.Get<string>("TYPE");
		MaterialPointer material = MaterialFactory::CreateMaterial(type);

		if (!materialDesc.HasKey("PARAMS")) {
			LogError("Miss material params.");
			return;
		}
		CJsonObject params = materialDesc.Get<CJsonObject>("PARAMS");

		if (params.HasKey("MAPS")) {
			CJsonObject maps = params.Get<CJsonObject>("MAPS");
			int maps_size = maps.GetArraySize();
			for (int j = 0; j < maps_size; j++) {
				CJsonObject map = maps[j];
				string key = map.Get<string>("KEY");
				string value = map.Get<string>("VALUE");
				TexturePointer tex = assets->GetTexture(value);
				material->SetTexture(key, tex);
			}
		}
		//setupMaterial(type, material, params,assets);
		assets->AddMaterial(name, material);
	}
}