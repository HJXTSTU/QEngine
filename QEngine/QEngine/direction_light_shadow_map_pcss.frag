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

#define LIGHT_WORLD_SIZE 1024

float CaculateShadow(vec3 FragPos, vec3 Normal, sampler2D LightDepthBuffer, mat4 LightSpaceMatrix,vec3 LightDir, vec2 NearFarPlane,vec2 LightSize, vec2 Bias){
	vec4 lightSpaceProjection = (LightSpaceMatrix*vec4(FragPos,1.0f));
	lightSpaceProjection /= lightSpaceProjection.w;
	lightSpaceProjection = lightSpaceProjection*0.5f+0.5f;
	//if(lightSpaceProjection.z > 1.0)
        //return 1.0f;

	// initPoissonSamples(lightSpaceProjection.xy);

	//	vec4 projectionWithNormalBias = (LightSpaceMatrix*vec4(FragPos+Normal,1.0f));
	//	projectionWithNormalBias /= projectionWithNormalBias.w;
	//	projectionWithNormalBias = projectionWithNormalBias*0.5f+0.5f;
	
	vec2 texelSize = textureSize(LightDepthBuffer, 0);
	//lightSpaceProjection.xy +=  normalize(projectionWithNormalBias.xy-lightSpaceProjection.xy)*NormalBias/texelSize.xy;
	
	

	// float bias = max(Bias.y*(1.0f-dot(Normal,LightDir)),Bias.x)/(NearFarPlane.y-NearFarPlane.x);
	float bias = 0.005f;
	float currentDepth = lightSpaceProjection.z;
	// float closestDepth = texture(LightDepthBuffer, lightSpaceProjection.xy).r;
	// return currentDepth<closestDepth?1.0f:0.0f;
	

//	PCSS
	float accum_blocker_depth = 0.0f;
	int num_blockers = 0;
	float zReciver = currentDepth-bias;
	int sample_count = 0;
	for(float x = -2.0f;x<=2.0f;x+=1.0f){
		for(float y = -2.0f;y<=2.0f;y+=1.0f){
			float sampleDepth = texture(LightDepthBuffer, lightSpaceProjection.xy + vec2(x,y)/texelSize.xy).r;
			if (sampleDepth < zReciver ) {
				accum_blocker_depth += sampleDepth;
				num_blockers +=1;
			}
			sample_count += 1;
		}
	}
	//for( int i = 0; i < BLOCKER_SEARCH_NUM_SAMPLES; i++ ) {
	//	float shadowMapDepth = texture2D(LightDepthBuffer, lightSpaceProjection.xy + poissonDisk[i]*searchRadius/texelSize).r;
	//	if ( shadowMapDepth < zReciver ) {
	//		accum_blocker_depth += shadowMapDepth;
	//		num_blockers +=1;
	//	}
	//}
	if(num_blockers==0){
		return 1.0f;
	}

	//if(num_blockers>=sample_count/2)return 0.0f;
	

	float avg_blocker_depth = accum_blocker_depth/float(sample_count);

	float penumbraRatio = (zReciver - avg_blocker_depth)/(avg_blocker_depth);
	penumbraRatio = max(0,penumbraRatio);

	float filterRadius = penumbraRatio;
	vec2 filterSize = vec2(filterRadius*LightSize.x,filterRadius*LightSize.y)/texelSize.xy;
	float shadow = 0;
	vec2 filterDelta = filterSize*0.2f;
	
	sample_count = 0;
	for(float x = -filterSize.x;x<=filterSize.x;x+=filterDelta.x){
		for(float y = -filterSize.y;y<=filterSize.y;y+=filterDelta.y){
			float sampleDepth = texture(LightDepthBuffer, lightSpaceProjection.xy + vec2(x,y)).r;
			if( zReciver >= sampleDepth ) shadow += 1.0;
			sample_count += 1;
		}
	}
	if(sample_count>0)
		shadow/= float(sample_count);

	shadow = min(max(shadow,0.0f),1.0f);

	return 1 - shadow;
}


void main(){

	float depth = texture2D(DepthBuffer, TexCoords).r;
	//	DepthToWorldPosition
	vec4 NDC = vec4(TexCoords.x * 2.0 - 1.0, TexCoords.y * 2.0 - 1.0, depth * 2.0 - 1, 1.0);
	vec4 worldPos = inverse(view) * inverse(projection) * NDC;
	worldPos /= worldPos.w;
	float distance = length(worldPos.xyz-viewPos);
	if(distance >= 999.0||distance<0.0f)discard;

	vec3 normal = texture2D(NormalBuffer, TexCoords).xyz;
	
	float s = 1.0f;
	
	float offset = 0.0f;
	float ratio = distance/(CameraFar-CameraNear);
	if(ratio<=CascadeSplits[0]){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer1, LightSpaceMatrix1, -LightDirection, NearFarPlane1,LightSize1, LightBias);
	}else if(ratio<=CascadeSplits[1]){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer2, LightSpaceMatrix2, -LightDirection, NearFarPlane2,LightSize2, LightBias);
	}else if(ratio<=CascadeSplits[2]){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer3, LightSpaceMatrix3, -LightDirection, NearFarPlane3,LightSize3, LightBias);
	}else if(ratio<=CascadeSplits[3]){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer4, LightSpaceMatrix4, -LightDirection, NearFarPlane4,LightSize4, LightBias);
	}
	FragColor = vec4(s,s,s,1.0f);
}