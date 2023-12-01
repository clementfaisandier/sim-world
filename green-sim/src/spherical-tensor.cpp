#include <stdlib.h>
#include <cmath>

#include <stdio.h>

#define PI 3.14159265
#define D_TO_RAD 0.01745329251

#include "spherical-tensor.h"


/*

Defines a SphericalTensor
Resolution defines the number of sub-divisions in each direction: 360 meaning each subdivision is 1 degree -> classic
lon_res is therefore the number of longitudonal subdivision (number of resulting longitudonal lines)
same thing for latitudes



*/


void SphericalTensor::DefineBuffers() {

	// allocation

	int vertex_count = lon_res * (lat_res - 1) + 2; // num vertex
	int triangle_count = lon_res * (lat_res - 1) * 2; // num indices (num squares  -(*2)> num triangles -(*3)> num indices)

	vertex_buffer_size = vertex_count * sizeof(*vertex_buffer) * 3; // xyz so 3
	index_buffer_size = triangle_count * sizeof(*index_buffer) * 3; // 3 verteces per triangle

	vertex_buffer = (float*)malloc(vertex_buffer_size);
	index_buffer = (unsigned int*)malloc(index_buffer_size);

	if (vertex_buffer == NULL) {
		printf("Unable to allocate vertex_buffer. Error: %d\n", errno);
		exit(errno);
	}
	if (index_buffer == NULL) {
		printf("Unable to allocate index_buffer. Error: %d\n", errno);
		exit(errno);
	}

	printf("vertex_count: %d, triangle_count: %d\nvertex_buffer_size: %u, index_buffer_size: %u\n", vertex_count, triangle_count, vertex_buffer_size, index_buffer_size);

	// vertex assignment

	AssignVertices();
	AssignIndices();

	// indice assignment

	// north pole cap

	int indice_i = 0;

	for (int i = 1; i < lon_res;) {
		index_buffer[indice_i++] = 0;
		index_buffer[indice_i++] = i;
		index_buffer[indice_i++] = ++i;
	}
	index_buffer[indice_i++] = 0;
	index_buffer[indice_i++] = 1;
	index_buffer[indice_i++] = lon_res;

	// mid strips

	// go through strips
	for (int lat_i = 0; lat_i < lat_res - 3; lat_i++) {

		int top_i = (lat_i * lon_res) + 1;
		int bot_i = top_i + lon_res;

		// doing the squares on each strips into triangles
		for (int i = 0; i < lon_res; i++) {
			index_buffer[indice_i++] = top_i;
			index_buffer[indice_i++] = bot_i;
			index_buffer[indice_i++] = ++bot_i;

			index_buffer[indice_i++] = bot_i;
			index_buffer[indice_i++] = top_i;
			index_buffer[indice_i++] = ++top_i;
		}

		index_buffer[indice_i++] = top_i;
		index_buffer[indice_i++] = bot_i;
		bot_i = bot_i - lat_res + 1;
		index_buffer[indice_i++] = bot_i; // original bot_i

		index_buffer[indice_i++] = bot_i;
		index_buffer[indice_i++] = top_i;
		index_buffer[indice_i++] = top_i - lat_res + 1;
	}

	int south_pole = (lon_res * (lat_res - 1)) + 1;

	// south pole cap
	for (int i = south_pole - lon_res; i < south_pole - 1;) {
		index_buffer[indice_i++] = south_pole;
		index_buffer[indice_i++] = i;
		index_buffer[indice_i++] = ++i;
	}
	index_buffer[indice_i++] = south_pole;
	index_buffer[indice_i++] = south_pole - lon_res;
	index_buffer[indice_i++] = south_pole - 1;
}

void SphericalTensor::AssignVertices() {

	int vertex_i = 0;

	float lon_separation = (((float)DEFAULT_LONGITUDE_RESOLUTION) / lon_res) * D_TO_RAD;
	float lat_separation = (((float)DEFAULT_LATITUDE_RESOLUTION) / lat_res) * D_TO_RAD;

	float lon_limit = DEFAULT_LONGITUDE_RESOLUTION * D_TO_RAD;
	float lat_limit = DEFAULT_LATITUDE_RESOLUTION * D_TO_RAD;

	float lat_start = PI / 2;

	printf("limits: %f %f\n", lon_limit, lat_limit);

	// north pole
	vertex_buffer[vertex_i++] = 0.0; // x
	vertex_buffer[vertex_i++] = 0.0; // y
	vertex_buffer[vertex_i++] = 1.0; // z

	for (int lat_i = 1; lat_i < lat_res; lat_i++) { // defines non-pole vertices

		float lat = (PI / 2) - (lat_separation * lat_i);

		for (int lon_i = 0; lon_i < lon_res; lon_i++) {

			float lon = lon_separation * lon_i;

			vertex_buffer[vertex_i++] = cos(lat) * sin(lon); // x
			vertex_buffer[vertex_i++] = cos(lat) * cos(lon); // y
			vertex_buffer[vertex_i++] = sin(lat); // z

			printf("i: %d, lon: %f, lat: %f, x: %f, y: %f, z: %f\n", (vertex_i / 3) - 1, lon, lat, vertex_buffer[vertex_i - 3], vertex_buffer[vertex_i - 2], vertex_buffer[vertex_i - 1]);
		}
	}

	// south pole
	vertex_buffer[vertex_i++] = 0.0; // x
	vertex_buffer[vertex_i++] = 0.0; // y
	vertex_buffer[vertex_i++] = -1.0; // z
}


void SphericalTensor::AssignIndices() {
	
	// north cap

	// everything in the middle




	//south cap

}





SphericalTensor::SphericalTensor(float resolution) {
	lon_res = (int)(DEFAULT_LONGITUDE_RESOLUTION * resolution);
	lat_res = (int)(DEFAULT_LATITUDE_RESOLUTION * resolution);

	DefineBuffers();
}

SphericalTensor::SphericalTensor(int lon_res, int lat_res) {
	SphericalTensor::lon_res = lon_res;
	SphericalTensor::lat_res = lat_res;
	DefineBuffers();
}

SphericalTensor :: ~SphericalTensor() {
	free(vertex_buffer);
	free(index_buffer);
}

float* SphericalTensor::GetVertexBuffer() {
	return vertex_buffer;
}

unsigned int* SphericalTensor::GetIndexBuffer() {
	return index_buffer;
}