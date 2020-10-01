#version 420 core
layout (location = 0) out vec3 gNormal;

//in mat3 TBN;
in vec3 Normal;
//in vec3 Tangents;

void main(){
	gNormal = Normal;
	//gTangents = Tangents;
}