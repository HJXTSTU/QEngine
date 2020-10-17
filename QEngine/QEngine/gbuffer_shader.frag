#version 420 core
layout (location = 0) out vec3 gVertexNormal;
layout (location = 1) out vec3 gNormalMap;

in vec3 VertexNormal;
in mat3 TBN;
in vec2 UV;

uniform int useNormalMap;
uniform sampler2D normalMap;

void main(){
	gVertexNormal = VertexNormal;
	if(useNormalMap==1){
		vec3 normal = texture2D(normalMap, UV).xyz;
		normal = normalize(inverse(TBN)*normalize(normal* 2.0 - 1.0));
		gNormalMap = normal;
	}else{
		gNormalMap = VertexNormal;
	}
}