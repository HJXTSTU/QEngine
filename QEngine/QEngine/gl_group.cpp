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

ObjectType Group::GetType() {
	return ObjectType::GROUP_OBJECT;
}