#pragma once
#include "gl_assets.h"
#include "CJsonObject.hpp"
#include <string>
using namespace std;
using namespace neb;

GLuint parseParam(std::string value);

void loadConfigTextures(std::shared_ptr<AssetsManager> assets, CJsonObject textures);

void createMaterials(std::shared_ptr<AssetsManager> assets, CJsonObject materials);

void loadModels(std::shared_ptr<AssetsManager> assets, CJsonObject models);