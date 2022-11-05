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

float CaculateShadow(vec3 FragPos, vec3 Normal, sampler2D LightDepthBuffer, mat4 LightSpaceMatrix,vec3 LightDir, vec2 NearFarPlane,vec2 LightSize, vec2 Bias){
	if(dot(LightDir,Normal)<=0)return 0.3f;
	vec4 lightSpaceProjection = (LightSpaceMatrix*vec4(FragPos,1.0f));
	lightSpaceProjection /= lightSpaceProjection.w;
	lightSpaceProjection = lightSpaceProjection*0.5f+0.5f;

	vec4 projectionWithNormalBias = (LightSpaceMatrix*vec4(FragPos+Normal,1.0f));
	projectionWithNormalBias /= projectionWithNormalBias.w;
	projectionWithNormalBias = projectionWithNormalBias*0.5f+0.5f;
	
	vec2 texelSize = textureSize(LightDepthBuffer, 0);
	lightSpaceProjection.xy +=  normalize(projectionWithNormalBias.xy-lightSpaceProjection.xy)*NormalBias/texelSize.xy;
	
	

	float bias = max(Bias.y*(1.0f-dot(Normal,LightDir)),Bias.x)/(NearFarPlane.y-NearFarPlane.x);
	//float bias = 0.05f/(NearFarPlane.y-NearFarPlane.x);
	//float bias = Bias.y;
	float currentDepth = lightSpaceProjection.z;
	float closestDepth = texture(LightDepthBuffer, lightSpaceProjection.xy).r;

//	PCSS
	
	float accum_blocker_depth = 0.0f;
	float num_blockers = 0.0f;
	float lightSize = LightSize.x;
	float biased_depth = currentDepth - bias;
	int window = 3;
	for(int i=-window;i<=window;i++){
		for(int j=-window;j<=window;j++){
			vec2 offset = vec2(i,j)*lightSize/texelSize;
			float sampleDepth = texture(LightDepthBuffer, lightSpaceProjection.xy + offset).r;
			if(sampleDepth < biased_depth){
				num_blockers += 1.0f;
				accum_blocker_depth += sampleDepth;
			}
		}
	}

	if(num_blockers==(window*2+1)*(window*2+1)){
		return 0.0f;
	}

	if(num_blockers==0.0f){
		return 1.0f;
	}

	float avg_blocker_depth = accum_blocker_depth/num_blockers;

	float z_vs = NearFarPlane.x + (NearFarPlane.y-NearFarPlane.x)*currentDepth;
	float avg_depth_vs = NearFarPlane.x + (NearFarPlane.y-NearFarPlane.x)*avg_blocker_depth;

	float penumbra_size = lightSize * (z_vs - avg_depth_vs)/avg_depth_vs;
	
	// if(penumbra_size==0.0f)return 0;
	
	float shadow = 0.0f;
	
	int range = int(penumbra_size);
	
	range = range > 6 ? 6: range;
    range = range < 2 ? 2 : range;

	for(int x = -range; x <= range; ++x)
    {
        for(int y = -range; y <= range; ++y)
        {
			vec2 offset = vec2(x, y) / texelSize;
            float pcfDepth = texture(LightDepthBuffer, lightSpaceProjection.xy + offset).r;
            
			shadow += (biased_depth<pcfDepth?1.0f:0.0f);
        }
    }

	shadow /= (range*2+1)*(range*2+1);

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