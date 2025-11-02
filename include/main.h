#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <iostream>
#include <fstream>
#include <errno.h>

#include "spherical-mesh-builder.h"
#include "transformation-module.h"


static void glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

static void glfwErrorCallback(int code, const char* description);

static char* parseShader(const char* filepath);

static unsigned int compileShader(unsigned int type, char* sourceCode);

static unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);


