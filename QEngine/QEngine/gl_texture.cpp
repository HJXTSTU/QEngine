#include "gl_texture.h"
#include <iostream>
TextureData::TextureData(const char *filename) {
	this->m_ucData = stbi_load(filename, &this->m_iWidth, &this->m_iHeight, &this->m_iNumComponents, 0);
	if (m_iNumComponents == 3) {
		m_uiColorFormat = GL_RGB;
	}
	else if (m_iNumComponents == 4) {
		m_uiColorFormat = GL_RGBA;
	}
	if (!m_ucData) {
		std::cout << "Load image " << filename << " faile." << std::endl;
	}

	m_bLoaded = true;
}

void TextureData::Load(const char * filename) {
	this->m_ucData = stbi_load(filename, &this->m_iWidth, &this->m_iHeight, &this->m_iNumComponents, 0);
	if (m_iNumComponents == 3) {
		m_uiColorFormat = GL_RGB;
	}
	else if (m_iNumComponents == 4) {
		m_uiColorFormat = GL_RGBA;
	}
	if (!m_ucData) {
		std::cout << "Load image " << filename << " faile." << std::endl;
	}

	m_bLoaded = true;
}


unsigned char * TextureData::GetRawData() {
	return m_ucData;
}

Color TextureData::GetColor(int w, int h) {
	Color res;
	int stride = (h*m_iWidth + w) * m_iNumComponents;
	res.r = m_ucData[stride];
	res.g = m_ucData[stride + 1];
	res.b = m_ucData[stride + 2];
	res.r /= 255.0f;
	res.g /= 255.0f;
	res.b /= 255.0f;
	return res;
}

Color TextureData::GetColor(float u, float v) {
	int w = m_iWidth * u;
	int h = m_iHeight * v;
	return GetColor(w, h);
}

TextureData::~TextureData() {
	if (this->m_ucData) {
		if (this->m_bLoaded)
			stbi_image_free(this->m_ucData);
		else
			delete[] this->m_ucData;
	}
}

Texture::Texture(const char *filepath,
	GLuint warp_s, GLuint warp_t,
	GLuint min_filter, GLuint mag_filter) :m_texRawData(filepath) {
	if (m_texRawData.GetRawData()) {
		glGenTextures(1, &m_uiTextureId);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warp_s);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warp_t);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
		glTexImage2D(GL_TEXTURE_2D, 0, m_texRawData.GetColorFormat(), m_texRawData.GetWidth(), m_texRawData.GetHeight(), 0, m_texRawData.GetColorFormat(), GL_UNSIGNED_BYTE, m_texRawData.GetRawData());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_strPath = std::string(filepath);
		std::cout << "[LOG]:" << "Genterated a texture: " << m_strPath << std::endl;
	}
}

TexturePointer Texture::AsPtr()
{
	return shared_from_this();
}

void Texture::Use(GLuint index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
}

Texture::~Texture() {
	if (!m_uiTextureId) {
		glDeleteTextures(1, &m_uiTextureId);
	}

}
