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

uniform float CameraFar;
uniform float CameraNear;
uniform float CascadeSplits[4];

uniform vec3 LightDirection;

uniform sampler2D LightDepthBuffer1;
uniform mat4 LightSpaceMatrix1;
uniform vec2 NearFarPlane1;
uniform vec2 LightSize1;

uniform sampler2D LightDepthBuffer2;
uniform mat4 LightSpaceMatrix2;
uniform vec2 NearFarPlane2;
uniform vec2 LightSize2;

uniform sampler2D LightDepthBuffer3;
uniform mat4 LightSpaceMatrix3;
uniform vec2 NearFarPlane3;
uniform vec2 LightSize3;

uniform sampler2D LightDepthBuffer4;
uniform mat4 LightSpaceMatrix4;
uniform vec2 NearFarPlane4;
uniform vec2 LightSize4;

uniform float NormalBias;
uniform vec2  LightBias;
uniform float Intensity;

#define LIGHT_WORLD_SIZE 0.005

#define NUM_SAMPLES 17
#define NUM_RINGS 11
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES

vec2 poissonDisk[NUM_SAMPLES];

float rand(vec2 co){
	return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void initPoissonSamples( const in vec2 randomSeed ) {
	float PI = 3.1415926;
	float PI2 = PI*PI;
	float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
	float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

	// jsfiddle that shows sample pattern: https://jsfiddle.net/a16ff1p7/
	float angle = rand( randomSeed ) * PI2;
	float radius = INV_NUM_SAMPLES;
	float radiusStep = radius;

	for( int i = 0; i < NUM_SAMPLES; i ++ ) {
		poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
		radius += radiusStep;
		angle += ANGLE_STEP;
	}
}

float unpackDepth(vec4 rgbaDepth){
	const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
	float depth = dot(rgbaDepth, bitShift);
	return depth;
}


float CaculateShadow(vec3 FragPos, vec3 Normal, sampler2D LightDepthBuffer, mat4 LightSpaceMatrix,vec3 LightDir, vec2 NearFarPlane,vec2 LightSize, vec2 Bias,float offsetScale){
	vec4 lightSpaceProjection = (LightSpaceMatrix*vec4(FragPos,1.0f));
	lightSpaceProjection /= lightSpaceProjection.w;
	lightSpaceProjection = lightSpaceProjection*0.5f+0.5f;
	
	vec2 texelSize = textureSize(LightDepthBuffer, 0).xy;
	
	
	vec2 uv = lightSpaceProjection.xy;
	//float bias = max(Bias.y*(1.0f-dot(Normal,LightDir)),Bias.x)/(NearFarPlane.y-NearFarPlane.x);
	float bias = 0.005f;
	float currentDepth = lightSpaceProjection.z;
	float closestDepth = texture(LightDepthBuffer, uv).r;
	float biased_depth = currentDepth - bias;
	float shadow = 0.0f;

	vec2 offset = fract(uv.xy*offsetScale);
	offset.y += offset.x;
	// offset.y^=offset.x;
	if(offset.y>1.1f)offset.y=0.0f;

	for(float x = -2f;x<=2.0f;x+=1.0f){
		for(float y = -2;y<=2.0f;y+=1.0f){
			float sampleDepth = texture(LightDepthBuffer, uv + vec2(x,y)/texelSize.xy).r;
			shadow += biased_depth>sampleDepth?0.0f:1.0f;
		}
	}
	
	shadow/= 25.0f;
	return shadow;

}


void main(){

	float depth = texture2D(DepthBuffer, TexCoords).r;
	//	DepthToWorldPosition
	vec4 NDC = vec4(TexCoords.x * 2.0 - 1.0, TexCoords.y * 2.0 - 1.0, depth * 2.0 - 1, 1.0);
	vec4 worldPos = inverse(view) * inverse(projection) * NDC;
	worldPos /= worldPos.w;
	float distance = length(worldPos.xyz-viewPos);
	if(distance >= 999.0||distance<0.0f)discard;

	vec3 normal = normalize(texture2D(NormalBuffer, TexCoords).xyz);
	
	float s = 1.0f;
	
	float offset = 0.0f;
	float ratio = distance/(CameraFar-CameraNear);
	if(ratio<=CascadeSplits[0]){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer1, LightSpaceMatrix1, -LightDirection, NearFarPlane1,LightSize1, LightBias,0.5f);
	}else if(ratio<=CascadeSplits[1]){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer2, LightSpaceMatrix2, -LightDirection, NearFarPlane2,LightSize2, LightBias,0.25f);
	}else if(ratio<=CascadeSplits[2]){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer3, LightSpaceMatrix3, -LightDirection, NearFarPlane3,LightSize3, LightBias,0.15f);
	}else if(ratio<=CascadeSplits[3]){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer4, LightSpaceMatrix4, -LightDirection, NearFarPlane4,LightSize4, LightBias,0.05f);
	}
	FragColor = vec4(s,s,s,max(dot(normal,-LightDirection),0.0f));
}