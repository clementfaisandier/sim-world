#include <stdlib.h>
#include <cmath>

#include <stdio.h>


/*

Defines a SphericalTensor
Resolution defines the number of sub-divisions in each direction: 360 meaning each subdivision is 1 degree -> classic
lon_res is therefore the number of longitudonal subdivision (number of resulting longitudonal lines)
same thing for latitudes



*/

class SphericalTensor {

private:
	float* vertex_buffer;
	unsigned int* indice_buffer;

	unsigned int vertex_buffer_size;
	unsigned int indice_buffer_size;

	unsigned int lon_res;
	unsigned int lat_res;

	void DefineBuffers() {

		// allocation

		int vertex_count = ((lon_res - 1) * (lat_res - 2) + 2); // num vertex
		int indice_count = lon_res * (lat_res-1) * 6; // num indices

		vertex_buffer_size = vertex_count * sizeof(vertex_buffer) * 3; // xyz so 3
		indice_buffer_size = indice_count * sizeof(indice_buffer);

		vertex_buffer = (float*)malloc(vertex_buffer_size);
		indice_buffer = (unsigned int*) malloc(indice_buffer_size);

		// vertex assignment

		int vertex_i = 0;

		float lon = 0;
		float lat = 90;

		float lon_separation = ((float)DEFAULT_LONGITUDE_RESOLUTION) / lon_res;
		float lat_separation = ((float)DEFAULT_LATITUDE_RESOLUTION) / lat_res;

		// north pole, just avoiding a bunch of identical vertices here
		vertex_buffer[vertex_i++] = cos(lat) * sin(lon); // x
		vertex_buffer[vertex_i++] = cos(lat) * cos(lon); // y
		vertex_buffer[vertex_i++] = sin(lat); // z

		for (lat += lat_separation; lat > -(signed int)(DEFAULT_LATITUDE_RESOLUTION/2); lat -= lat_separation) {
			for (; lon < DEFAULT_LONGITUDE_RESOLUTION; lon += lon_separation) {
				vertex_buffer[vertex_i++] = cos(lat) * sin(lon); // x
				vertex_buffer[vertex_i++] = cos(lat) * cos(lon); // y
				vertex_buffer[vertex_i++] = sin(lat); // z
			}
		}

		// south pole, just avoiding a bunch of identical vertices here
		vertex_buffer[vertex_i++] = cos(lat) * sin(lon); // x
		vertex_buffer[vertex_i++] = cos(lat) * cos(lon); // y
		vertex_buffer[vertex_i++] = sin(lat); // z

		for (int i = 0; i < vertex_count; i++) {
			printf("%f\n", vertex_buffer[i]);
		}

		// indice assignment

		// north pole cap

		int indice_i = 0;

		for (int i = 1; i < lon_res;) {
			indice_buffer[indice_i++] = 0;
			indice_buffer[indice_i++] = i;
			indice_buffer[indice_i++] = ++i;
		}
		indice_buffer[indice_i++] = 0;
		indice_buffer[indice_i++] = 1;
		indice_buffer[indice_i++] = lon_res;

		// mid strips

		// go through strips
		for (int lat_i = 0; lat_i < lat_res - 3; lat_i++) {

			int top_i = (lat_i * lon_res) + 1;
			int bot_i = top_i + lon_res;

			// doing the squares on each strips into triangles
			for (int i = 0; i < lon_res; i++) {
				indice_buffer[indice_i++] = top_i;
				indice_buffer[indice_i++] = bot_i;
				indice_buffer[indice_i++] = ++bot_i;

				indice_buffer[indice_i++] = bot_i;
				indice_buffer[indice_i++] = top_i;
				indice_buffer[indice_i++] = ++top_i;
			}

			indice_buffer[indice_i++] = top_i;
			indice_buffer[indice_i++] = bot_i;
			bot_i = bot_i - lat_res + 1;
			indice_buffer[indice_i++] = bot_i; // original bot_i

			indice_buffer[indice_i++] = bot_i;
			indice_buffer[indice_i++] = top_i;
			indice_buffer[indice_i++] = top_i - lat_res + 1;
		}
		
		int south_pole = (lon_res * (lat_res - 1)) + 1;

		// south pole cap
		for (int i = south_pole-lon_res; i < south_pole-1;) {
			indice_buffer[indice_i++] = south_pole;
			indice_buffer[indice_i++] = i;
			indice_buffer[indice_i++] = ++i;
		}
		indice_buffer[indice_i++] = south_pole;
		indice_buffer[indice_i++] = south_pole - lon_res;
		indice_buffer[indice_i++] = south_pole-1;
	}



public:
	const unsigned int DEFAULT_LONGITUDE_RESOLUTION = 360;
	const unsigned int DEFAULT_LATITUDE_RESOLUTION = 180;

	SphericalTensor(float resolution) {
		lon_res = (int) (DEFAULT_LONGITUDE_RESOLUTION * resolution);
		lat_res = (int) (DEFAULT_LATITUDE_RESOLUTION * resolution);

		DefineBuffers();
	}

	~SphericalTensor() {
		free(vertex_buffer);
		free(indice_buffer);
	}

	float* GetVertexBuffer() {
		return vertex_buffer;
	}

	unsigned int* GetIndexBuffer() {
		return indice_buffer;
	}
};