#version 450 core

layout(location = 0) out vec4 f_color;

uniform vec4 u_color;
uniform sampler2D u_texture;

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec3 v_position;


void main()
{
    //vec4 color = vec4(cross( vec3(u_color), vec3(v_color)), 1);
    //vec4 texture_color = texture(u_texture, v_uvs);
    //vec4 xs = vec4(cross(vec3(texture_color) , vec3(v_color)), 1) * u_color;
    
    vec3 view = normalize(-v_position);
    vec3 light = normalize(vec3(1,1,1));

    vec3 normal = normalize(v_normal);
    vec3 color  = vec3(0.5f,0.2f,0.04f);
    vec3 reflection = reflect(-light, normal);

    vec3 ambiant  = color * 0.2;
    vec3 diffuse  = max(dot(normal, light),0) * color;
    vec3 specular = pow(max(dot(reflection, view),0),4.0)* color;



    f_color = vec4(ambiant + diffuse + specular, 1);
}