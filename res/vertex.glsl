#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

uniform float aspect_ratio; 
uniform float Time; 

uniform mat4 view_projection_matrix;

out vec3 vertex_position;
out vec2 uv;


void main()
{
    vertex_position = in_position;
    uv = in_uv;
    //gl_Position = vec4(in_position.x / aspect_ratio , in_position.y, 0., 1.);
    gl_Position = view_projection_matrix * vec4(in_position, 1.);
}



