R"(
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

uniform mat4 translation_matrix;

out vec4 out_color;

void main()
{
    // The w component (1.0) is required for clip space
    gl_Position = translation_matrix * vec4(position, 1.0);
    out_color = color;
}
)"
