#pragma once



class SphericalTensor {

private:
	float* vertex_buffer;
	unsigned int* index_buffer;

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

	SphericalTensor(float Resolution);
	SphericalTensor(int lon_res, int lat_res);
	
	~SphericalTensor();

	float* GetVertexBuffer();
	unsigned int* GetIndexBuffer();
};