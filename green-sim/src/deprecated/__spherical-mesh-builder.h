#pragma once
#include "mesh.h"
#include "compute-mesh.h"

#include <iostream>
#include <glm/trigonometric.hpp>

#define DEBUG false

#define PI 3.14159265358
#define H_PI 1.57079632679
#define D_TO_RAD 0.01745329251

class SphericalMeshBuilder {

private:

	Mesh surface_mesh = Mesh();
	ComputeMesh athmospheric_mesh = ComputeMesh();

	unsigned int num_lon;
	unsigned int num_lat;
	unsigned int num_layers;

	float lon_step;
	float lat_step;
	float layer_step;

	float scale_min;
	float scale_max;

	void InitFields(float scale_min, float scale_max, unsigned int num_lon, unsigned int num_lat, unsigned int num_layers);

	void InitSurfaceMesh();

	void InitAthmosphericMesh();

	void DefineSurfaceVertices();

	void DefineSurfaceIndices();

	void DefineAthmosphericMesh();

	void DefineAthmosphericMeshGraphicVertices();

	void DefineAthmosphericGraphicIndices();

public:

	SphericalMeshBuilder(float scale_min, float scale_max, unsigned int num_lon, unsigned int num_lat, unsigned int num_layers);

	Mesh* GetSurfaceMesh();
	ComputeMesh* GetComputeMesh();

	void Print();

};
