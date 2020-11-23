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

float CaculateShadow(vec3 FragPos, vec3 Normal, sampler2D LightDepthBuffer, mat4 LightSpaceMatrix,vec3 LightDir, vec2 NearFarPlane,vec2 LightSize, vec2 Bias){
	vec4 lightSpaceProjection = (LightSpaceMatrix*vec4(FragPos,1.0f));
	lightSpaceProjection /= lightSpaceProjection.w;
	lightSpaceProjection = lightSpaceProjection*0.5f+0.5f;

	initPoissonSamples(lightSpaceProjection.xy);

	vec4 projectionWithNormalBias = (LightSpaceMatrix*vec4(FragPos+Normal,1.0f));
	projectionWithNormalBias /= projectionWithNormalBias.w;
	projectionWithNormalBias = projectionWithNormalBias*0.5f+0.5f;
	
	vec2 texelSize = textureSize(LightDepthBuffer, 0);
	lightSpaceProjection.xy +=  normalize(projectionWithNormalBias.xy-lightSpaceProjection.xy)*NormalBias/texelSize.xy;
	
	

	float bias = max(Bias.y*(1.0f-dot(Normal,LightDir)),Bias.x)/(NearFarPlane.y-NearFarPlane.x);
	//float bias = 0.05f/(NearFarPlane.y-NearFarPlane.x);
	//float bias = Bias.y;
	float currentDepth = lightSpaceProjection.z;
	// float closestDepth = texture(LightDepthBuffer, lightSpaceProjection.xy).r;

	

//	PCSS
	float accum_blocker_depth = 0.0f;
	float num_blockers = 0.0f;
	float zReciver = currentDepth;
	float searchRadius = (LIGHT_WORLD_SIZE/LightSize.x)/zReciver;
	float biased_depth = currentDepth - bias;
	for( int i = 0; i < BLOCKER_SEARCH_NUM_SAMPLES; i++ ) {
		float shadowMapDepth = unpackDepth(texture2D(LightDepthBuffer, lightSpaceProjection.xy + poissonDisk[i] * searchRadius));
		if ( shadowMapDepth < zReciver ) {
			accum_blocker_depth += shadowMapDepth;
			num_blockers +=1.0f;
		}
	}
	

	
	

	if(num_blockers==NUM_SAMPLES){
		return 0.0f;
	}

	if(num_blockers==0.0f){
		return 1.0f;
	}

	float avg_blocker_depth = accum_blocker_depth/num_blockers;

	float penumbraRatio = (zReciver - avg_blocker_depth)/(avg_blocker_depth);
	float filterRadius = penumbraRatio * (LIGHT_WORLD_SIZE/LightSize.x);

	float shadow = 0.0f;
	for(int i=0;i < PCF_NUM_SAMPLES; i++ ){
		float depth = unpackDepth(texture2D(LightDepthBuffer, lightSpaceProjection.xy + poissonDisk[ i ] * filterRadius ));
		if( biased_depth <= depth ) shadow += 1.0;
	}
	for(int i=0;i < PCF_NUM_SAMPLES; i++ ){
		float depth = unpackDepth(texture2D(LightDepthBuffer, lightSpaceProjection.xy + -poissonDisk[ i ].yx * filterRadius ));
		if( biased_depth <= depth ) shadow += 1.0;
	}
	shadow/= 2*PCF_NUM_SAMPLES;

	if(lightSpaceProjection.z > 1.0)
        shadow = 0.0;

	shadow = min(max(shadow,0.0f),1.0f);

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

	vec3 normal = texture2D(NormalBuffer, TexCoords).xyz;
	
	float s = 1.0f;
	
	float offset = 0.0f;
	if(distance<= CameraFar*CascadeSplits[0]){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer1, LightSpaceMatrix1, -LightDirection, NearFarPlane1,LightSize1, LightBias);
	}else if(distance<=CameraFar*(CascadeSplits[0]+CascadeSplits[1])){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer2, LightSpaceMatrix2, -LightDirection, NearFarPlane2,LightSize2, LightBias);
	}else if(distance<=CameraFar*(CascadeSplits[0]+CascadeSplits[1]+CascadeSplits[2])){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer3, LightSpaceMatrix3, -LightDirection, NearFarPlane3,LightSize3, LightBias);
	}else if(distance<=CameraFar*(CascadeSplits[0]+CascadeSplits[1]+CascadeSplits[2]+CascadeSplits[3])){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer4, LightSpaceMatrix4, -LightDirection, NearFarPlane4,LightSize4, LightBias);
	}
	FragColor = vec4(s,s,s,1.0f);
}