#version 420 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D NormalBuffer;

uniform sampler2D DepthBuffer;

uniform vec3 LightColor;

uniform vec3 Position;

uniform float Constant;

uniform float Linear;

uniform float Quadratic;

layout(std140, binding = 0) uniform Matrics{
						//	Base Alignment		Aligned Offset
	mat4 projection;	//		64					0
	mat4 view;			//		64					64
	vec3 viewPos;		//		16					128
						//	Total: 144 bytes
};

void main(){
	vec3 N = texture2D(NormalBuffer,TexCoords).xyz;

	float depth = texture2D(DepthBuffer,TexCoords).r;

	//	DepthToWorldPosition
	vec4 NDC = vec4(TexCoords.x * 2.0 - 1.0, TexCoords.y * 2.0 - 1.0, depth * 2.0 - 1, 1.0);
	vec4 worldPos = inverse(view) * inverse(projection) * NDC;
	worldPos /= worldPos.w;
	if(length(vec3(worldPos)-viewPos)>=999.0)discard;

	float distance = length(Position-worldPos.xyz);
	
	float attenuation = 1.0 / (Constant + Linear * distance + Quadratic * (distance * distance));

	vec3 L = normalize(Position-worldPos.xyz);

	float NdL = max(dot(N,L),0.0f);

	vec3 V = normalize(viewPos-vec3(worldPos));
	vec3 H = normalize(V+L);
	float NdH = max(dot(N,H),0.0f);

	FragColor = vec4(NdL*LightColor*attenuation,NdH*attenuation);
}
