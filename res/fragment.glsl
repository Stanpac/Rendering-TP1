#version 410

out vec4 out_color;

uniform float Time; 

in vec3 vertex_position;

void main()
{
    // out_color = vec4(0.84, 0.83, 0.11,1. );
    out_color = vec4(vertex_position, 1.);
}