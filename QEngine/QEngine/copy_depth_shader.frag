#version 420 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthTex;
float LinearizeDepth(float depth)
{
	float near = 0.1;
    float far = 100.0;
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near) / (far + near - z * (far - near));
}
void main(){
	float depth = texture2D(depthTex,TexCoords).r;
	FragColor = vec4(depth,depth,depth,1);
}