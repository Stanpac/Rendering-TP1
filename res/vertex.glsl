#version 410

layout(location = 0) in vec3 in_position;

uniform float aspect_ratio; 
uniform float Time; 
uniform mat4 view_projection_matrix;

void main()
{
    gl_Position = vec4(in_position.x / aspect_ratio , in_position.y, 0., 1.);
    gl_Position = view_projection_matrix * vec4(in_position, 1.);
}



