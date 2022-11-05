#pragma once
#include <glad/glad.h>
#include <string>
#include "constant.h"
using namespace std;

void UseTexture(GLuint texture_code, GLuint id);

void UseCubemap(GLuint texture_code, GLuint id);

char* ReadJsonFile(const string &path);