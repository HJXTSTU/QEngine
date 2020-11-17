#version 420 core
in vec4 FragPos;

uniform vec3 Position;

uniform float FarPlane;

void main()
{
    // get distance between fragment and light source
    float distance = length(FragPos.xyz -  Position);

    // map to [0;1] range by dividing by far_plane
    distance = distance / FarPlane;

    // write this as modified depth
    gl_FragDepth = distance;
}
