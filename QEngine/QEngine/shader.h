#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader {
private:
	std::string loadVertexShaderCode(const char *path);

	std::string loadGeometryShaderCode(const char *path);

	std::string loadFragmentShaderCode(const char *path);

	unsigned int generateShaderProgram(std::string &vCode, std::string &fCode, std::string &gCode);

	unsigned int compileVertexCode(std::string &vCode);

	unsigned int compileGeometryCode(std::string &gCode);

	unsigned int compileFragmentCode(std::string &fCode);

	unsigned int linkProgram(unsigned int vertex, unsigned int fragment, unsigned int geometry);

	bool checkCompileErrors(unsigned int shader, std::string type);

	bool checkLinkErrors(unsigned int shader, std::string type);

public:
	unsigned int ID;

	Shader();

	Shader(const char *vertexPath, const char *fragmentPath, const char* geometryPath = nullptr);

	void LoadShader(const char *vertexPath, const char *fragmentPath, const char* geometryPath = nullptr);

	// activate the shader
	// ------------------------------------------------------------------------
	void use();

	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const;
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const;
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const;
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const;

	void setVec3(const std::string &name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

};