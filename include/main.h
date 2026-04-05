#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

#include "Shaders.h"
#include "SphericalMeshBuilder.h"
#include "GridMeshBuilder.h"
#include "TransformationModule.h"

#define PI 3.14159265358979323846


int spherical_simulation(void);

static void glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

static void glfwErrorCallback(int code, const char* description);

static char* parseShader(const char* filepath);

static unsigned int compileShader(unsigned int type, const std::string& sourceCode);

static unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);


