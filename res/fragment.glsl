#version 410

out vec4 out_color;

uniform float Time; 
uniform sampler2D my_texture;

in vec3 vertex_position;
in vec2 uv;

void main()
{
    vec4 texture_color = texture(my_texture, uv);
    out_color = texture_color;
    //out_color = vec4(0.84, 0.83, 0.11,1. );
    //out_color = vec4(vertex_position, 1.);
}