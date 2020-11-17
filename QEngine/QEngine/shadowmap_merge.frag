#version 420 core
out vec4 OutputColor;

in vec2 TexCoords;

uniform sampler2D inputShadowmap;

uniform int LightCount;

void main(){
	float s = texture(inputShadowmap,TexCoords).r/(LightCount*5);
	OutputColor = vec4(s,s,s,0.0f);
}