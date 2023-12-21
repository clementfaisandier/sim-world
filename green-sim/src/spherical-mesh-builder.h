#pragma once
#include "mesh.h"

#include <iostream>
#include <glm/trigonometric.hpp>

#define DEBUG false

#define PI 3.14159265358
#define H_PI 1.57079632679
#define D_TO_RAD 0.01745329251

class SphericalMeshBuilder {

private:

	Mesh mesh = Mesh();

	unsigned int num_lon;
	unsigned int num_lat;
	unsigned int num_depth;

	float lon_step;
	float lat_step;

	float scale;

	void InitFields(float scale, unsigned int num_lon, unsigned int num_lat);

	void DefineSurfaceVertices();

	void DefineSurfaceIndices();

public:

	SphericalMeshBuilder(float scale, unsigned int num_lon, unsigned int num_lat);

	Mesh* GetMesh();

	void Print();

};
