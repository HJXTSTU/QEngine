#version core 420
out vec4 FragColor;

uniform sampler2D diffuseMap;
in vec2 UV;
void main(){
	FragColor = texture2D(diffuseMap, UV.xy);
}