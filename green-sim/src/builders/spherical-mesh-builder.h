/*

num_lon, lat, layers dictate the number of cell/vertices/volumes per mesh

The current compute mesh is not an offset grid, unlike what was originally envisioned, meaning
	the compute cells are on the vertices, not  the surfaces of the meshe.


*/




#pragma once

#include <iostream>
#include "glm/vec3.hpp"
#include "glm/trigonometric.hpp"

#define N_ATTR_P_VERTEX 3
#define N_VERTEX_P_PRIMITIVE 3


// Meshes and mesh Functions	

typedef struct SphericalGraphicsMesh {

	// NOTE: for the future, it would be clearer to store vertices and indices as lists of vec3
	float* vertex_buffer = nullptr;
	unsigned int* index_buffer = nullptr;

	unsigned int vertex_buffer_size = 0;
	unsigned int index_buffer_size = 0;

	unsigned int vertex_buffer_count = 0;
	unsigned int index_buffer_count = 0; // num primitive draw calls

	unsigned int num_lon = 0;
	unsigned int num_lat = 0;
	unsigned int num_layers = 0;

	void Print();
	void PrintMeta();
};

typedef struct SphericalComputeMesh {

	typedef struct Cell {
		glm::vec3 velocity;
		float pressure;
		float density;
	};

	Cell* compute_buffer = nullptr;

	unsigned int compute_buffer_size = 0;

	unsigned int compute_buffer_count = 0;

	unsigned int num_lon = 0;
	unsigned int num_lat = 0;
	unsigned int num_layers = 0;

	void Print();
	void PrintMeta();
};

class SphericalMeshBuilder {

public:

	SphericalMeshBuilder(unsigned int num_lon, unsigned int num_lat, unsigned int num_layers, float scale_min, float scale_max);

	SphericalGraphicsMesh* GetSurfaceMesh();
	SphericalGraphicsMesh* GetAthmosphericMesh();

	SphericalComputeMesh* GetComputeMesh();

private:

	unsigned int num_lon = 0;
	unsigned int num_lat = 0;
	unsigned int num_layers = 0;

	float scale_min = 0;
	float scale_max = 0;

	float lon_step;
	float lat_step;
	float layer_step; // scale adjusted

	/*
	* Param:
	*	vertex_buffer: pre-allocated vertex buffer
	* or
	*	index_buffer: pre-allocated index buffer
	* Return: 
	*	(int) number of vertices defined
	* or
	*	(int) number of primitives defined
	*/
	int DefineSurfaceVertexBuffer(float* vertex_buffer, float scale);
	int DefineSurfaceIndexBuffer(unsigned int* index_buffer);

	int DefineAthmosphereVertexBuffer(float* vertex_buffer);
	void DefineAthmosphereComponentVertices(float* vertex_buffer, unsigned int* vbi, float x, float y, float z);
	int DefineAthmosphereIndexBuffer(unsigned int* index_buffer);
	void DefineAthmosphericVolume(unsigned int* index_buffer, unsigned int* ibi, unsigned int layer_offset);

	int DefineComputeBuffer(SphericalComputeMesh::Cell* compute_buffer);


};
