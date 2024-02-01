#version 430 core

layout(location = 0) out vec4 color;

in vec4 p_color;

void main()
{
    color = p_color;
}