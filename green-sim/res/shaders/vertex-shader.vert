#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 translation_matrix;
uniform ivec3 u_dimension;
uniform vec4 u_color;
uniform int u_object;

out vec4 p_color;

void main()
{
    vec4 fposition = vec4(position, 1.0);
    gl_Position = translation_matrix * fposition;

    p_color = color;
}