#pragma once

#include <GL/glew.h>

#include <vector>

#include "Attribute.h"

struct Entity {

	GLint location;
	std::vector<Attribute> attributes;
};