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

uniform float Interval;
uniform vec3 LightDirection;

uniform sampler2D LightDepthBuffer1;
uniform mat4 LightSpaceMatrix1;

uniform sampler2D LightDepthBuffer2;
uniform mat4 LightSpaceMatrix2;

uniform sampler2D LightDepthBuffer3;
uniform mat4 LightSpaceMatrix3;

float CaculateShadow(vec3 FragPos, vec3 Normal, sampler2D LightDepthBuffer, mat4 LightSpaceMatrix,vec3 LightDir){
	vec4 lightSpaceProjection = (LightSpaceMatrix*vec4(FragPos,1.0f));
	lightSpaceProjection /= lightSpaceProjection.w;
	lightSpaceProjection = lightSpaceProjection*0.5f+0.5f;
	
	float currentDepth = lightSpaceProjection.z;
	float closestDepth = texture2D(LightDepthBuffer, lightSpaceProjection.xy).r;
	
	float bias = max(0.0001 * (1-dot(normalize(Normal), normalize(-LightDir))), 0.000);
	
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(LightDepthBuffer, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture2D(LightDepthBuffer, lightSpaceProjection.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 0.0 : 1.0;        
        }    
    }
    shadow /= 9.0;
	//shadow = currentDepth - bias  > closestDepth?0.0f:1.0f;
	return shadow;
}


void main(){

	float depth = texture2D(DepthBuffer, TexCoords).r;
	//	DepthToWorldPosition
	vec4 NDC = vec4(TexCoords.x * 2.0 - 1.0, TexCoords.y * 2.0 - 1.0, depth * 2.0 - 1, 1.0);
	vec4 worldPos = inverse(view) * inverse(projection) * NDC;
	worldPos /= worldPos.w;
	float distance = length(vec3(worldPos)-viewPos);
	if(distance >= 999.0)discard;

	vec3 normal = texture2D(NormalBuffer, TexCoords).xyz;
	
	float s = 1.0f;
	if(distance<=Interval){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer1, LightSpaceMatrix1, LightDirection);
	}else if(distance<=Interval*2.0f){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer2, LightSpaceMatrix2, LightDirection);
	}else if(distance<=Interval*3.0f){
		s = CaculateShadow(worldPos.xyz, normal, LightDepthBuffer3, LightSpaceMatrix3, LightDirection);
	}
	FragColor = vec4(s,s,s,1.0f);
}