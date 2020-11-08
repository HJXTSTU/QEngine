#version 420 core
out vec4 OutputColor;

in vec2 TexCoords;

uniform sampler2D inputShadowmap;

void main(){
	float s = texture(inputShadowmap,TexCoords).r;
	OutputColor = vec4(s,s,s,0.0f);
}