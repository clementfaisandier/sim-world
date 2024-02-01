/*

num_lon & num_lat represent the number of lon/lat divisions TODO: make this the num of distinct lat, for lon this is already the case
num_layers is the number of layers


*/




#pragma once

#include <iostream>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/trigonometric.hpp"

#define N_VERTEX_P_PRIMITIVE 3


// Meshes and mesh Functions	

typedef struct SphericalGraphicsMesh {

	glm::vec3* vertex_buffer = nullptr;
	glm::vec4* color_buffer = nullptr;
	unsigned int* index_buffer = nullptr;

	unsigned int vertex_buffer_size = 0;
	unsigned int color_buffer_size = 0;
	unsigned int index_buffer_size = 0;

	unsigned int vertex_buffer_count = 0;
	unsigned int color_buffer_count = 0;
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
	int DefineSurfaceVertexBuffer(glm::vec3* vertex_buffer, float scale);
	int DefineSurfaceColorBuffer(glm::vec4* color_buffer);
	int DefineSurfaceIndexBuffer(unsigned int* index_buffer);

	int DefineAthmosphereVertexBuffer(glm::vec3* vertex_buffer);
	void DefineAthmosphereComponentVertices(glm::vec3* vertex_buffer, unsigned int* vbi, float x, float y, float z);

	int DefineAthmosphereColorBuffer(glm::vec4* color_buffer);
	void DefineAthmosphereComponentColors(glm::vec4* color_buffer, unsigned int* cbi);

	int DefineAthmosphereIndexBuffer(unsigned int* index_buffer);
	void DefineAthmosphericVolume(unsigned int* index_buffer, unsigned int* ibi, unsigned int layer_offset);

	int DefineComputeBuffer(SphericalComputeMesh::Cell* compute_buffer);


};
