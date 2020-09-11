#version 420 core 
out vec4 FragColor;

uniform sampler2D diffuseMap;
in vec2 UV;
void main(){
	FragColor = texture2D(diffuseMap, UV.xy);
}