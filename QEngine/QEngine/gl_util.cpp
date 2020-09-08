#include "gl_util.h"

void UserTexture(GLuint texture_code, GLuint id) {
	glActiveTexture(texture_code);
	glBindTexture(GL_TEXTURE_2D, id);
}
