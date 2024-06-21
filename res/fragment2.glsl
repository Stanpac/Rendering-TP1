#version 410

out vec4 out_color;

uniform sampler2D my_texture;

in vec2 uv;

void main()
{
    vec4 texture_color = texture(my_texture, uv);
    float gray = dot(texture_color.rgb, vec3(0.3));
    out_color = vec4 (vec3 (gray), texture_color.a);
    //out_color =  vec4 (1 - texture_color.rgb, 1.) ;
}