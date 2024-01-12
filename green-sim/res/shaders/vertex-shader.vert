#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 translation_matrix;

uniform vec4 u_color;

const vec4 colors[5] = vec4[5] (vec4(1, 1, 1, 1),
                                vec4(1, 0, 0, 1),
                                vec4(0, 1, 0, 1),
                                vec4(0, 0, 1, 1),
                                vec4(0, 0, 0, 1));

out vec4 p_color;

void main()
{
    gl_Position = translation_matrix * position;
    p_color = colors[gl_VertexID];
}