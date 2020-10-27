#include "gl_group.h"
Group::Group() {

}

void Group::OnGBufferRender(Shader &gbufferShader) {
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnGBufferRender(gbufferShader);
	}
}

void Group::OnSurfaceRender() {
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnSurfaceRender();
	}
}

void Group::OnSurfaceRender(const RenderTexture &lightBuffer) {
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnSurfaceRender(lightBuffer);
	}
}

void Group::OnSurfaceRender(const RenderTexture &lightBuffer, const RenderTexture &shadowmap) {
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnSurfaceRender(lightBuffer, shadowmap);
	}
}

void Group::OnShadowmapRender(Shader& shader) {
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->OnShadowmapRender(shader);
	}
}

ObjectType Group::GetType() {
	return ObjectType::GROUP_OBJECT;
}