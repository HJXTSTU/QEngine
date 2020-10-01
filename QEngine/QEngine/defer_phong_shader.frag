#version 420 core 
out vec4 FragColor;

uniform int UseDiffuseMap;
uniform sampler2D diffuseMap;

uniform int UseSpecularMap;
uniform sampler2D specularMap;

uniform int UseNormalMap;
uniform sampler2D normalMap;

in vec2 UV;
in vec3 FragPos;
in vec3 ViewPos;

uniform sampler2D LightBuffer;


void main(){
	vec3 diffuseColor = vec3(1,1,1);
	if(UseDiffuseMap==1)
		diffuseColor = texture2D(diffuseMap, UV.xy).rgb;
	
	vec3 specularColor = vec3(1,1,1);
	if(UseSpecularMap==1)
		specularColor = texture2D(specularMap, UV.xy).rgb;
	
	vec2 screenSize = vec2(1024.0,1024.0);
	vec4 light = texture2D(LightBuffer,gl_FragCoord.xy/screenSize);

	vec3 result = light.rgb*diffuseColor+light.a*specularColor;

	FragColor = vec4(result, 1.0f);
} 