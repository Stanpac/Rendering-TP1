#version 410

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_uv;

out vec2 uv;

void main()
{
    //vertex_position = in_position;
    uv = in_uv;
    gl_Position = vec4(in_position.x , in_position.y, 0., 1.);
    //gl_Position = view_projection_matrix * vec4(in_position, 1.);
}
