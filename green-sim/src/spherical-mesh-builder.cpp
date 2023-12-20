#include "spherical-mesh-builder.h"



SphericalMeshBuilder::SphericalMeshBuilder(float inner_bound, unsigned int num_lon, unsigned int num_lat) {

	if (inner_bound == 0 || num_lon == 0 || num_lat == 0) {
		std::cout << "Error: SphericalMeshBuilder: Invalid Input\n";
		std::exit(1);
	}

	InitFields(inner_bound, inner_bound, num_lon, num_lat, 1);

	DefineSurfaceVertices(mesh.vertex_buffer, 0);
	DefineSurfaceIndices();
}

SphericalMeshBuilder::SphericalMeshBuilder(float inner_bound, float outer_bound, unsigned int num_lon, unsigned int num_lat, unsigned int num_depth) {
	
	if (inner_bound == 0 || outer_bound < inner_bound || num_lon == 0 || num_lat == 0 || num_depth == 0) {
		std::cout << "Error: SphericalMeshBuilder: Invalid Input\n";
		std::exit(1);
	}

	InitFields(inner_bound, outer_bound, num_lon, num_lat, num_depth);

	DefineVolumeVertices();
}

void SphericalMeshBuilder::InitFields(float inner_bound, float outer_bound, unsigned int num_lon, unsigned int num_lat, unsigned int num_depth) {

	this->num_lon = num_lon;
	this->num_lat = num_lat;
	this->num_depth = num_depth;

	this->lon_step = 360 / num_lon;
	this->lat_step = 360 / num_lat;
	this->depth_step = (outer_bound - inner_bound) / num_depth;

	this->inner_bound = inner_bound;
	this->outer_bound = outer_bound;

	this->num_lon = num_lon;
	this->num_lat = num_lat;

	mesh.vertex_num_components = N_VERTEX_COMPONENTS;
	mesh.vertex_size = sizeof(*mesh.vertex_buffer) * N_VERTEX_COMPONENTS;

	mesh.primary_count = num_lon;
	mesh.secondary_count = num_lat;
	mesh.tertiary_count = num_depth;

	mesh.vertex_buffer_count = (num_lon * (num_lat - 1) + 2) * num_depth;
	mesh.index_buffer_count = (num_lon * (num_lat - 1) * 2) * num_depth; // only applicable to current index setup

	mesh.vertex_buffer_size = mesh.vertex_buffer_count * mesh.vertex_size;
	mesh.index_buffer_size = sizeof(*mesh.index_buffer) * N_PRIMITIVE_COMPONENTS * mesh.index_buffer_count;

	mesh.vertex_buffer = (float*)malloc(mesh.vertex_buffer_size);
	mesh.index_buffer = (unsigned int*)malloc(mesh.index_buffer_size);

	if (mesh.vertex_buffer == NULL || mesh.index_buffer == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate vertex/index buffer.\nVB: %u, IB: %u\n", mesh.vertex_buffer, mesh.index_buffer);
		exit(errno);
	}
}

void SphericalMeshBuilder::DefineSurfaceVertices(float* vertex_buffer, int depth_i) {

	int vertex_i = 0;

	int r = inner_bound + (depth_i * depth_step);

	// north pole
	vertex_buffer[vertex_i++] = 0.0; // x
	vertex_buffer[vertex_i++] = 0.0; // y
	vertex_buffer[vertex_i++] = r; // z

	for (int lat_i = 1; lat_i < num_lat; lat_i++) { // defines non-pole vertices

		float lat = H_PI - (lat_step * lat_i);

		for (int lon_i = 0; lon_i < num_lat; lon_i++) {

			float lon = lon_step * lon_i;

			vertex_buffer[vertex_i++] = cos(lat) * sin(lon) * r; // x
			vertex_buffer[vertex_i++] = cos(lat) * cos(lon) * r; // y
			vertex_buffer[vertex_i++] = sin(lat) * r; // z
		}
	}

	// south pole
	vertex_buffer[vertex_i++] = 0.0; // x
	vertex_buffer[vertex_i++] = 0.0; // y
	vertex_buffer[vertex_i++] = -r; // z
}

void SphericalMeshBuilder::DefineVolumeVertices() {
	
	// TODO
}

void SphericalMeshBuilder::DefineSurfaceIndices() {

	if (num_depth != 1)
		return;

	unsigned int* index_buffer = mesh.index_buffer;

	unsigned int buffer_i = 0;

	unsigned int top_vertex_i = 0;
	unsigned int bot_vertex_i = 1;

	for (int lat_i = 0; lat_i < num_lat; lat_i++) {

		unsigned int initial_top_vertex = top_vertex_i;
		unsigned int initial_bot_vertex = bot_vertex_i;

		for (int lon_i = 1; lon_i <= num_lon; lon_i++) {

			if (lat_i == 0) {
				index_buffer[buffer_i++] = top_vertex_i;
				index_buffer[buffer_i++] = bot_vertex_i++;
				index_buffer[buffer_i++] = lon_i == num_lat ? initial_bot_vertex : bot_vertex_i;

				if (lon_i == num_lon)
					top_vertex_i++;

				if (DEBUG) printf("i: %d, a: %d, b: %d, c: %d\n", (buffer_i / 3) - 1, index_buffer[buffer_i - 3], index_buffer[buffer_i - 2], index_buffer[buffer_i - 1]);

			}
			else if (lat_i != num_lat - 1) {

				unsigned int top_left = top_vertex_i++;
				unsigned int top_right = lon_i == num_lon ? initial_top_vertex : top_vertex_i; // this longitude trick takes care of wrapping
				unsigned int bot_left = bot_vertex_i++;
				unsigned int bot_right = lon_i == num_lon ? initial_bot_vertex : bot_vertex_i;

				index_buffer[buffer_i++] = top_left;
				index_buffer[buffer_i++] = bot_left;
				index_buffer[buffer_i++] = bot_right;

				index_buffer[buffer_i++] = top_left;
				index_buffer[buffer_i++] = bot_right;
				index_buffer[buffer_i++] = top_right;

				if (DEBUG) printf("i: %d, a: %d, b: %d, c: %d, d: %d\n", (buffer_i / 3) - 2, top_left, top_right, bot_left, bot_right);

			}
			else {
				index_buffer[buffer_i++] = bot_vertex_i;

				// test if with depth this solution works with face-culling, the other would not work with face-culling
				//index_buffer[buffer_i++] = lon_i == 1 ? lon_i : bot_vertex_i + 1 - lon_i;
				//index_buffer[buffer_i++] = bot_vertex_i - lon_i;

				index_buffer[buffer_i++] = top_vertex_i++;
				index_buffer[buffer_i++] = lon_i == num_lon ? initial_top_vertex : top_vertex_i;
			}
		}
	}
}

Mesh* SphericalMeshBuilder::GetMesh() {
	return &mesh;
}