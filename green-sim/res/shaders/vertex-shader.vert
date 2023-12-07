#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 translation_matrix;
uniform mat4 x_rot_matrix;
uniform mat4 y_rot_matrix;
uniform mat4 z_rot_matrix;
uniform mat4 scale_matrix;

uniform vec4 u_color;

out vec4 color;

void main()
{
    gl_Position = scale_matrix * translation_matrix * x_rot_matrix * y_rot_matrix * z_rot_matrix *  position;
    color = u_color;
}