#version 330 core

layout(location = 0) in
vec4 position;

uniform mat4 translation_matrix;
uniform mat4 x_rot_matrix;
uniform mat4 y_rot_matrix;
uniform mat4 z_rot_matrix;

void main()
{
    gl_Position = translation_matrix * x_rot_matrix * y_rot_matrix * z_rot_matrix *  position;
}