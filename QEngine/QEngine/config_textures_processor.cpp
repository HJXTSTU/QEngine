#include "loader_util.h"
GLuint parseParam(std::string value) {
	if (value == "REPEAT") {
		return GL_REPEAT;
	}
	else if (value == "CLAMP") {
		return GL_CLAMP;
	}
	else if (value == "CLAMP_TO_EDGE") {
		return GL_CLAMP_TO_EDGE;
	}
	else if (value == "CLAMP_TO_BORDER") {
		return GL_CLAMP_TO_BORDER;
	}
	else if (value == "LINEAR") {
		return GL_LINEAR;
	}
	else if (value == "REPEAT") {
		return GL_REPEAT;
	}
	else {
		cout << "[ERROR]:" << "Invilid params. " << "[" << value << "]" << endl;
	}
}

void loadConfigTextures(std::shared_ptr<AssetsManager> assets, CJsonObject textures) {
	int size = textures.GetArraySize();
	for (int i = 0; i < size; i++) {
		CJsonObject textureDesc = textures[i];
		std::string path = textureDesc.Get<string>("PATH");
		std::string id = textureDesc.Get<string>("NAME");
		if (!textureDesc.HasKey("PARAMS")) {
			Texture *tex = new Texture(path.c_str());
			assets->AddTexture(id, tex->AsPtr());
		}
		else {
			CJsonObject params = textureDesc.Get<CJsonObject>("PARAMS");
			GLuint warp_s = GL_REPEAT;
			GLuint warp_t = GL_REPEAT;
			GLuint min_filter = GL_LINEAR;
			GLuint mag_filter = GL_LINEAR;
			if (params.HasKey("WARP_S")) {
				warp_s = parseParam(params.Get<string>("WARP_S"));
			}
			if (params.HasKey("WARP_T")) {
				warp_t = parseParam(params.Get<string>("WARP_t"));
			}
			if (params.HasKey("MIN_FILTER")) {
				min_filter = parseParam(params.Get<string>("MIN_FILTER"));
			}
			if (params.HasKey("MAG_FILTER")) {
				mag_filter = parseParam(params.Get<string>("MAG_FILTER"));
			}
			TexturePointer tex = TexturePointer(new Texture(path.c_str(), warp_s, warp_t, min_filter, mag_filter));
			assets->AddTexture(id, tex->AsPtr());
		}
	}
}