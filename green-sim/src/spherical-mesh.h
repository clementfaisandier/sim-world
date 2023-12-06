#pragma once

#define DEBUG false


class SphericalMesh {

private:
	float* vertex_buffer;
	unsigned int* index_buffer;

	unsigned int vertex_buffer_count;
	unsigned int index_buffer_count;

	unsigned int vertex_buffer_size;
	unsigned int index_buffer_size;

	unsigned int lon_res;
	unsigned int lat_res;

	void DefineBuffers();

	void AssignVertices();
	void AssignIndices();

public:
	const unsigned int DEFAULT_LONGITUDE_RESOLUTION = 360;
	const unsigned int DEFAULT_LATITUDE_RESOLUTION = 180;

	SphericalMesh(float Resolution);
	SphericalMesh(int lon_res, int lat_res);
	
	~SphericalMesh();

	float* GetVertexBuffer();
	unsigned int* GetIndexBuffer();

	unsigned int GetVertexBufferCount();
	unsigned int GetIndexBufferCount();

	unsigned int GetVertexBufferSize();
	unsigned int GetIndexBufferSize();

	void PrintDrawOrder();
};