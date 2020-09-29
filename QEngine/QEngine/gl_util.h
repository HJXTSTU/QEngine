#pragma once
#include <glad/glad.h>
#include <string>
using namespace std;

void UseTexture(GLuint texture_code, GLuint id);

char* ReadJsonFile(const string &path);