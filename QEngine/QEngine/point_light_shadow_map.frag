#version 420 core
out vec4 FragColor;

in vec2 TexCoords;

layout(std140, binding = 0) uniform Matrics{
						//	Base Alignment		Aligned Offset
	mat4 projection;	//		64					0
	mat4 view;			//		64					64
	vec3 viewPos;		//		16					128
						//	Total: 144 bytes
};

uniform sampler2D DepthBuffer;
uniform sampler2D NormalBuffer;

uniform samplerCube LightDepthBuffer;

uniform vec3 Position;

uniform float FarPlane;

uniform float NormalBias;
uniform vec2  LightBias;

float CaculateShadow(vec3 FragPos, vec3 Normal, vec2 Bias){
	FragPos = FragPos + Normal * NormalBias;

	vec3 LightToFrag = FragPos - Position;
	if(length(LightToFrag)>=FarPlane)return 1.0f;
	if(dot(-LightToFrag,Normal)<=0)return 0.0f;

	float bias = max(Bias.y*(1.0f-dot(Normal,LightToFrag)),Bias.x);
	
	float currentDepth = length(Position-FragPos);

	int samples = 20;
	float viewDistance = length(viewPos - FragPos);
	float diskRadius = (1.0 + (viewDistance / FarPlane)) / 25.0;
	vec3 sampleOffsetDirections[20] = vec3[]
	(
		vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
		vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
		vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
		vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
		vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	);

	float shadow = 0.0f;
	for(int i = 0; i < samples; ++i)
	{
		//float closestDepth = texture(LightDepthBuffer, LightToFrag).r;
		float closestDepth = texture(LightDepthBuffer, LightToFrag + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= FarPlane;   // Undo mapping [0;1]
		if(currentDepth - bias < closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);

	return shadow;
}

void main(){
	float depth = texture2D(DepthBuffer, TexCoords).r;
	//	DepthToWorldPosition
	vec4 NDC = vec4(TexCoords.x * 2.0 - 1.0, TexCoords.y * 2.0 - 1.0, depth * 2.0 - 1, 1.0);
	vec4 worldPos = inverse(view) * inverse(projection) * NDC;
	worldPos /= worldPos.w;
	float distance = length(worldPos.xyz-viewPos);
	if(distance >= 999.0)discard;

	vec3 normal = normalize(texture2D(NormalBuffer, TexCoords).xyz);

	float s =  CaculateShadow(worldPos.xyz, normal, LightBias);
	FragColor = vec4(s,s,s,1.0f);
}

