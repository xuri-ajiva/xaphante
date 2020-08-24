#version 330 core

layout(location = 0) out vec4 f_color;

uniform vec4 u_color;
uniform sampler2D u_texture;

in vec4 v_color;
in vec2 v_uvs;


void main()
{
    vec4 color = vec4(cross(vec3(v_color) , vec3(u_color)), 1);
    vec4 texture_color = texture(u_texture, v_uvs);
    vec4 xs = vec4(cross(vec3(texture_color) , vec3(v_color)), 1) * u_color;
    f_color =   color;
}