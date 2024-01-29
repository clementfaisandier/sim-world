#pragma once

#include <GL/glew.h>
#include <vector>

#include "GL-component.h"


class Process {

private:
	GLuint program;
	std::vector<GLComponent> gl_components;

public:

	Process(GLuint program);
	~Process();

	void AddGLComponent(Entity entity);
	void RemoveGLComponent(Entity entity);

	void Run();


};