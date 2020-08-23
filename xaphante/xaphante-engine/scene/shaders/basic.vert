#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uvs;
layout(location = 2) in vec4 a_color;

out vec4 v_color;
out vec2 v_uvs;

uniform mat4 u_modelViewProje;

void main()
{
    gl_Position = u_modelViewProje * vec4(a_position, 1.0f) ;
    v_color = a_color;
    v_uvs = a_uvs;
}