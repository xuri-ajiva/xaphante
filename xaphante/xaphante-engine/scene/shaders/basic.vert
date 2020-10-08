#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

layout(location = 0) out vec3 v_normal;
layout(location = 1) out vec3 v_position;

uniform mat4 u_modelViewProje;
uniform mat4 u_modelView;
uniform mat4 u_invModelView;

void main()
{
    gl_Position = u_modelViewProje * vec4(a_position, 1.0f);

    v_normal   = mat3(u_invModelView) * a_normal;
    v_position = vec3(u_modelView * vec4(a_position, 1.0f));
}