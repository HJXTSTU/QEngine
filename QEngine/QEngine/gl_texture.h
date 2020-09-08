#pragma once
#include "stb_image.h"
#include "stb_image_write.h"
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <memory>
struct Color {
	float r;
	float g;
	float b;
};

class TextureData;
class Texture;

class TextureData {
private:
	unsigned char *m_ucData;
	int m_iWidth;
	int m_iHeight;
	int m_iNumComponents;
	unsigned int m_uiColorFormat;
	bool m_bLoaded;
public:
	TextureData(const char *filename);

	void Load(const char *filename);

	inline unsigned int GetWidth() { return m_iWidth; }

	inline unsigned int GetHeight() { return m_iHeight; }

	unsigned char * GetRawData();

	Color GetColor(int w, int h);

	Color GetColor(float u, float v);

	inline unsigned int GetColorFormat() { return m_uiColorFormat; }

	~TextureData();
};

typedef  std::shared_ptr<Texture> TexturePointer;

class Texture :public std::enable_shared_from_this<Texture> {
private:
	TextureData m_texRawData;
private:
	std::string m_strPath;
	GLuint m_uiTextureId;
public:
	explicit Texture(const char *filepath,
		GLuint warp_s = GL_REPEAT, GLuint warp_t = GL_REPEAT,
		GLuint min_filter = GL_LINEAR, GLuint mag_filter = GL_LINEAR);

	inline unsigned int GetTextureID() {
		return  this->m_uiTextureId;
	}


	TexturePointer AsPtr();

	void Use(GLuint active_number);

	~Texture();
};