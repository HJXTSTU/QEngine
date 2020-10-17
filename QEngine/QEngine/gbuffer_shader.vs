#version 420 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vUV;
layout (location = 3) in vec3 vTangents;

out vec3 VertexNormal;
out mat3 TBN;
out vec2 UV;

uniform mat4 model;
uniform mat3 normalMatrix;

layout(std140, binding = 0) uniform Matrics{
						//	Base Alignment		Aligned Offset
	mat4 projection;	//		64					0
	mat4 view;			//		64					64
	vec3 viewPos;		//		16					128
						//	Total: 144 bytes
};

void main(){
	gl_Position = projection*view*model*vec4(vPos,1.0f);

	VertexNormal = normalize(normalMatrix*vNorm);
	UV = vUV;

	vec3 T = normalize(vec3(model * vec4(vTangents,0.0f)));
    vec3 N = normalize(vec3(model * vec4(vNorm,0.0f)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N,T);
	TBN = transpose(mat3(T, B, N)); 
}