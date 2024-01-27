#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>

#include "builders/spherical-mesh-builder.h"
#include "builders/program-builder.h"
#include "transformation-module.h"


static void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

static void GLFWErrorCallback(int code, const char* description);
