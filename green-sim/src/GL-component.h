#pragma once

#include <GL/glew.h>

#include <vector>

struct GLComponent {

	GLint location;
	std::vector<Attribute> attributes;
};