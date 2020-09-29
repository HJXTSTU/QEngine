#version 420 core
layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec3 gTangents;

in vec3 Normal;
in vec3 Tangents;

void main(){
	gNormal = Normal;
	gTangents = Tangents;
}