#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>

#include "builders/spherical-mesh-builder.h"
#include "builders/program-builder.h"
#include "transformation-module.h"

#define PI 3.14159265358979323846


struct Settings {
    unsigned int GL_version_major = 4;
    unsigned int GL_version_minor = 3;

    unsigned int window_width = 640;
    unsigned int window_height = 480;


    int lon = 3;
    int lat = 3;
    int layers = 1;
    float scale_min = 1.0;
    float scale_max = 2.0;
};

static void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

static void GLFWErrorCallback(int code, const char* description);

static GLFWwindow* GLFWInitialization(Settings sts);

static void GLInitialization(Settings sts);