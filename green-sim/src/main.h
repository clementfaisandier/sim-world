#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>

#include "spherical-mesh-builder.h"
#include "transformation-module.h"


static void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

static void GLFWErrorCallback(int code, const char* description);

static char* ParseShader(const char* filepath);

static unsigned int CompileShader(unsigned int type, char* sourceCode);

static unsigned int CreateProgram(unsigned int vertexShader, unsigned int fragmentShader);


