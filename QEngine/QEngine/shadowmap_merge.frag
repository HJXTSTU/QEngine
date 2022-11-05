#version 420 core
out vec4 OutputColor;

in vec2 TexCoords;

uniform sampler2D inputShadowmap;

void main(){
	vec2 s = texture(inputShadowmap,TexCoords).ra;
	OutputColor = vec4(s.x,s.x,s.x,s.y);
}