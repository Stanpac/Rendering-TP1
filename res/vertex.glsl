#version 410

layout(location = 0) in vec2 in_position;

uniform float aspect_ratio; 
uniform float Time; 

void main()
{
    gl_Position = vec4(in_position.x / aspect_ratio , in_position.y, 0., 1.);
    //gl_Position.xy += vec2(sin(Time)/ aspect_ratio, cos(Time));
}



