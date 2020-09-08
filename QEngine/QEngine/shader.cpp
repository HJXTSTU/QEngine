#include "shader.h"


std::string Shader::loadVertexShaderCode(const char *path) {
	std::string vertexCode;
	std::ifstream vShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(path);
		std::stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();
		vertexCode = vShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		throw e;
	}
	return vertexCode;
}

std::string Shader::loadGeometryShaderCode(const char *path) {
	std::string geometryCode;
	std::ifstream gShaderFile;
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		gShaderFile.open(path);
		std::stringstream fShaderStream;
		fShaderStream << gShaderFile.rdbuf();
		geometryCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		throw e;
	}
	return geometryCode;
}

std::string Shader::loadFragmentShaderCode(const char *path) {
	std::string fragmentCode;
	std::ifstream fShaderFile;
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fShaderFile.open(path);
		std::stringstream fShaderStream;
		fShaderStream << fShaderFile.rdbuf();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		throw e;
	}
	return fragmentCode;
}

unsigned int Shader::generateShaderProgram(std::string &vCode, std::string &fCode, std::string &gCode) {
	unsigned int vertex = this->compileVertexCode(vCode);
	if (vertex == 0) {
		return 0;
	}
	unsigned int geometry = 0;
	if (gCode != "") {
		geometry = this->compileGeometryCode(gCode);
		if (geometry == 0) {
			return 0;
		}
	}

	unsigned int fragment = this->compileFragmentCode(fCode);
	if (fragment == 0) {
		return 0;
	}
	unsigned int program = this->linkProgram(vertex, fragment, geometry);
	if (program == 0) {
		return 0;
	}
	glDeleteShader(vertex);
	if (geometry)
		glDeleteShader(geometry);
	glDeleteShader(fragment);
	return program;
}

unsigned int Shader::compileVertexCode(std::string &vCode) {
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	const char *src = vCode.c_str();
	glShaderSource(vertex, 1, &src, NULL);
	glCompileShader(vertex);
	if (checkCompileErrors(vertex, "VERTEX")) {
		return vertex;
	}
	else {
		return 0;
	}
}

unsigned int Shader::compileGeometryCode(std::string &gCode) {
	unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);
	const char *src = gCode.c_str();
	glShaderSource(geometry, 1, &src, NULL);
	glCompileShader(geometry);
	if (checkCompileErrors(geometry, "GEOMETRY")) {
		return geometry;
	}
	else {
		return 0;
	}
}

unsigned int Shader::compileFragmentCode(std::string &fCode) {
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char *src = fCode.c_str();
	glShaderSource(fragment, 1, &src, NULL);
	glCompileShader(fragment);
	if (checkCompileErrors(fragment, "FRAGMENT")) {
		return fragment;
	}
	else {
		return 0;
	}
}

unsigned int Shader::linkProgram(unsigned int vertex, unsigned int fragment, unsigned int geometry) {
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex);
	if (geometry != 0)
		glAttachShader(program, geometry);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	if (checkLinkErrors(program, "PROGRAM")) {
		return program;
	}
	else {
		return 0;
	}
}

bool Shader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "COMPILE:" << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}
	return true;
}

bool Shader::checkLinkErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}
	return true;
}

Shader::Shader() :ID(0) {

}

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char* geometryPath) {
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	try {
		vertexCode = this->loadVertexShaderCode(vertexPath);
		if (geometryPath)
			geometryCode = this->loadGeometryShaderCode(geometryPath);
		fragmentCode = this->loadFragmentShaderCode(fragmentPath);
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	ID = generateShaderProgram(vertexCode, fragmentCode, geometryCode);
	if (ID == 0) {
		std::cout << "ERROR::SHADER::Generate shader program faile." << std::string(vertexPath) << std::endl;
		std::cout << "VertexPath:" << std::string(vertexPath) << "|FragmentPath:" << std::string(fragmentPath);
		if (geometryPath)
			std::cout << "|GeometryPath:" << std::string(geometryPath);
		std::cout << std::endl;
	}
}

void Shader::LoadShader(const char *vertexPath, const char *fragmentPath, const char* geometryPath) {
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	try {
		vertexCode = this->loadVertexShaderCode(vertexPath);
		if (geometryPath)
			geometryCode = this->loadGeometryShaderCode(geometryPath);
		fragmentCode = this->loadFragmentShaderCode(fragmentPath);
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	ID = generateShaderProgram(vertexCode, fragmentCode, geometryCode);
	if (ID == 0) {
		std::cout << "ERROR::SHADER::Generate shader program faile." << std::endl;
	}
}

// activate the shader
// ------------------------------------------------------------------------
void Shader::use()
{
	glUseProgram(ID);
}

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}