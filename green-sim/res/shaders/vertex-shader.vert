#version 330 core

layout(location = 0) in
vec4 position;

uniform mat4 translation_matrix;

void main()
{
    gl_Position = translation_matrix * position;
}