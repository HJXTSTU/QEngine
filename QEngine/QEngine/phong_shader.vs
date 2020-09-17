#version 420 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vUV;
layout (location = 3) in vec3 vTangents;

layout(std140, binding = 0) uniform Matrics{
						//	Base Alignment		Aligned Offset
	mat4 projection;	//		64					0
	mat4 view;			//		64					64
	vec3 viewPos;		//		16					128
						//	Total: 144 bytes
};

uniform mat4 model;
uniform mat3 normalMatrix;

out vec2 UV;
out vec3 FragPos;
out vec3 ViewPos;
out vec3 Norm;
out mat3 TBN;
void main(){
	gl_Position = projection * view * model * vec4(vPos, 1.0f);

    UV = vec2(vUV.x, vUV.y);
	FragPos = vec3(model * vec4(vPos, 1.0f));
	Norm = normalMatrix * vNorm;
	ViewPos = viewPos;
	
	vec3 T = normalize(normalMatrix * vTangents);
    vec3 N = normalize(normalMatrix * vNorm);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(T, N);
	mat3 TBN = transpose(mat3(T, B, N)); 
}