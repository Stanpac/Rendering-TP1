#version 410

layout(location = 0) in vec2 in_position;

uniform vec2 nom_de_votre_variable_uniforme; // Vous pouvez mettre le type que vous voulez, et le nom que vous voulez
void main()
{
    gl_Position = vec4(in_position + 0.4f, 0., 1.);
}