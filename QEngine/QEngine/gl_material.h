#pragma once
#include "shader.h"
#include <memory>
#include "gl_texture.h"
#include "gl_render_texture.h"
class Material;
typedef std::shared_ptr<Material> MaterialPointer;

class Material :public std::enable_shared_from_this<Material> {
protected:
	Shader m_shader;
public:
	virtual void Use(const glm::mat4 &model);

	virtual void Use(const glm::mat4 &model,const RenderTexture &lightBuffer);

	virtual void SetTexture(const std::string &key, TexturePointer tex) = 0;

	virtual MaterialPointer Clone() = 0;

	MaterialPointer AsSharedPtr();

	~Material();
};

