#version 420 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform float AdaptedIlumination;

vec3 ACESToneMapping(vec3 color, float adapted_ilumination)
{
    const float A = 2.51f;
    const float B = 0.03f;
    const float C = 2.43f;
    const float D = 0.59f;
    const float E = 0.14f;
    color *= adapted_ilumination;
    return (color * (A * color + B)) / (color * (C * color + D) + E);
}

void main(){
	vec3 color = texture(image,TexCoords).rgb;
	vec3 mapped = ACESToneMapping(color,AdaptedIlumination);

	const float gamma = 2.2;
	mapped = pow(mapped, vec3(1.0 / gamma));

	FragColor = vec4(mapped,1.0f);
}
