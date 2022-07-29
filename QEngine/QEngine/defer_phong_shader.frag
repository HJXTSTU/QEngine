#version 420 core 
out vec4 FragColor;

uniform int useDiffuseMap;
uniform sampler2D diffuseMap;

uniform int useSpecularMap;
uniform sampler2D specularMap;

uniform int useNormalMap;
uniform sampler2D normalMap;

in vec2 UV;
in vec3 FragPos;
in vec3 ViewPos;

uniform sampler2D LightBuffer;

uniform int UseShadowmap;
uniform sampler2D Shadowmap;


void main(){
	vec3 diffuseColor = vec3(1,1,1);
	if(useDiffuseMap==1)
		diffuseColor = texture2D(diffuseMap, UV.xy).rgb;
	
	vec3 specularColor = vec3(1,1,1);
	if(useSpecularMap==1)
		specularColor = texture2D(specularMap, UV.xy).rgb;
	
	vec2 screenSize = vec2(1024.0,1024.0);
	vec4 light = texture2D(LightBuffer,gl_FragCoord.xy/screenSize);

	float shadow=1.0f;
	if(UseShadowmap==1)
		shadow = max(min(texture2D(Shadowmap, gl_FragCoord.xy/screenSize).r,1.0f),0.0f);


	vec3 result = light.rgb*diffuseColor+max(pow(light.a,64),0.0f)*specularColor;
	
	vec3 ambient = vec3(0.0,0.0,0.0);

	FragColor = vec4((result)*shadow, 1.0f);
} 