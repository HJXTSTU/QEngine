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
in vec3 Norm;
in mat3 TBN;


vec3 CaculatePointLightIlumination(vec3 fragPos, vec3 p,float c,float l,float q,vec3 n,vec3 v,vec3 diffC,vec3 specC){
	float d = length(p-fragPos);
	vec3 L = normalize(p-fragPos);
	vec3 V = normalize(v-fragPos);
	
	vec3 H = normalize(V+L);

	float diff = max(dot(n,L),0.0f);
	float spec = max(pow(dot(n,H),64.0f),0.0f);
	
	float att = 1.0f/(c+l*d+q*d*d);
	return (diff*diffC+spec*specC)*att + vec3(0.1,0.1,0.1)*diffC;
	
}

void main(){
	vec3 diffuseColor = vec3(1,1,1);
	if(UseDiffuseMap==1)
		diffuseColor = texture2D(diffuseMap, UV.xy).rgb;
	
	vec3 specularColor = vec3(1,1,1);
	if(UseSpecularMap==1)
		specularColor = texture2D(specularMap, UV.xy).rgb;
	
	vec3 norm = Norm;
	vec3 fragPos = FragPos;
	vec3 viewPos = ViewPos;
	if(UseNormalMap==1){
		norm = texture(normalMap, UV.xy).rgb;
		norm = normalize(norm * 2.0 - 1.0);
		
		fragPos = TBN*FragPos;
		viewPos = TBN*ViewPos;
	}
	vec3 Illumination = CaculatePointLightIlumination(fragPos,viewPos,1.0f,0.35,0.44,norm,viewPos,diffuseColor,specularColor);
	

	FragColor = vec4(Illumination, 1.0f);
}