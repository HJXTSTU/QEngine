#include "gl_util.h"
#include <fstream>

void UserTexture(GLuint texture_code, GLuint id) {
	glActiveTexture(texture_code);
	glBindTexture(GL_TEXTURE_2D, id);
}

char* ReadJsonFile(const string &path) {
	ifstream fin("loading_config.json");
	fin.seekg(0, fin.end);
	unsigned int length = fin.tellg();
	fin.seekg(0, fin.beg);
	char *json = new char[length + 1];
	fin.read(json, length);
	return json;
}