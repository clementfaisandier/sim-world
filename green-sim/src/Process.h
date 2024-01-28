#pragma once

#include <GL/glew.h>
#include <vector>

#include "Entity.h"
#include "Attribute.h"


class Process {

private:
	GLuint program;
	std::vector<Entity> entities;

public:

	Process(GLuint program);
	~Process();

	void AddEntity(Entity entity);
	void RemoveEntity(Entity entity);

	void Run();


};