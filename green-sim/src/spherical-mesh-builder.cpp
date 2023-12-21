#include "spherical-mesh-builder.h"

SphericalMeshBuilder::SphericalMeshBuilder(float scale, unsigned int num_lon, unsigned int num_lat) {

	if (scale == 0 || num_lon == 0 || num_lat == 0) {
		std::cout << "Error: SphericalMeshBuilder: Invalid Input\n";
		std::exit(1);
	}

	InitFields(scale, num_lon, num_lat);

	DefineSurfaceVertices();
	DefineSurfaceIndices();
}

void SphericalMeshBuilder::InitFields(float scale, unsigned int num_lon, unsigned int num_lat) {

	this->num_lon = num_lon;
	this->num_lat = num_lat;
	this->num_depth = num_depth;

	this->lon_step = glm::radians(360.0 / num_lon);
	this->lat_step = glm::radians(180.0 / num_lat);

	this->scale = scale;

	this->num_lon = num_lon;
	this->num_lat = num_lat;

	mesh.vertex_num_components = N_VERTEX_COMPONENTS;
	mesh.vertex_size = sizeof(*mesh.vertex_buffer) * N_VERTEX_COMPONENTS;

	mesh.primary_count = num_lon;
	mesh.secondary_count = num_lat;
	mesh.tertiary_count = 1;

	mesh.vertex_buffer_count = (num_lon * (num_lat - 1) + 2);
	mesh.index_buffer_count = (num_lon * (num_lat - 1) * 2); // only applicable to current index setup

	mesh.vertex_buffer_size = mesh.vertex_buffer_count * mesh.vertex_size;
	mesh.index_buffer_size = sizeof(*mesh.index_buffer) * N_PRIMITIVE_COMPONENTS * mesh.index_buffer_count;

	mesh.vertex_buffer = (float*)malloc(mesh.vertex_buffer_size);
	mesh.index_buffer = (unsigned int*)malloc(mesh.index_buffer_size);

	if (mesh.vertex_buffer == NULL || mesh.index_buffer == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate vertex/index buffer.\nVB: %u, IB: %u\n", &mesh.vertex_buffer, &mesh.index_buffer);
		exit(errno);
	}
}

void SphericalMeshBuilder::DefineSurfaceVertices() {

	int vertex_i = 0;

	float* vertex_buffer = mesh.vertex_buffer;

	// north pole
	vertex_buffer[vertex_i++] = 0.0; // x
	vertex_buffer[vertex_i++] = 0.0; // y
	vertex_buffer[vertex_i++] = scale; // z

	for (int lat_i = 1; lat_i < num_lat; lat_i++) { // defines non-pole vertices

		float lat = H_PI - (lat_step * lat_i);

		for (int lon_i = 0; lon_i < num_lon; lon_i++) {

			float lon = lon_step * lon_i;

			vertex_buffer[vertex_i++] = cos(lat) * sin(lon) * scale; // x
			vertex_buffer[vertex_i++] = cos(lat) * cos(lon) * scale; // y
			vertex_buffer[vertex_i++] = sin(lat) * scale; // z
		}
	}

	// south pole
	vertex_buffer[vertex_i++] = 0.0; // x
	vertex_buffer[vertex_i++] = 0.0; // y
	vertex_buffer[vertex_i++] = -scale; // z
}

void SphericalMeshBuilder::DefineSurfaceIndices() {

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
				index_buffer[buffer_i++] = lon_i == num_lon ? initial_bot_vertex : bot_vertex_i + 1;
				index_buffer[buffer_i++] = bot_vertex_i++;

				if (lon_i == num_lon)
					top_vertex_i++;

				if (DEBUG) printf("i: %d, a: %d, b: %d, c: %d\n", (buffer_i / 3) - 1, index_buffer[buffer_i - 3], index_buffer[buffer_i - 2], index_buffer[buffer_i - 1]);
			}

			else if (lat_i != num_lat - 1) {

				unsigned int top_left = top_vertex_i++;
				unsigned int top_right = lon_i == num_lon ? initial_top_vertex : top_vertex_i; // this longitude trick takes care of wrapping
				unsigned int bot_left = bot_vertex_i++;
				unsigned int bot_right = lon_i == num_lon ? initial_bot_vertex : bot_vertex_i;

				index_buffer[buffer_i++] = bot_left;
				index_buffer[buffer_i++] = top_left;
				index_buffer[buffer_i++] = bot_right;

				index_buffer[buffer_i++] = bot_right;
				index_buffer[buffer_i++] = top_left;
				index_buffer[buffer_i++] = top_right;

				if (DEBUG) printf("i: %d, a: %d, b: %d, c: %d, d: %d\n", (buffer_i / 3) - 2, top_left, top_right, bot_left, bot_right);
			}

			else {
				index_buffer[buffer_i++] = bot_vertex_i;
				index_buffer[buffer_i++] = top_vertex_i++;
				index_buffer[buffer_i++] = lon_i == num_lon ? initial_top_vertex : top_vertex_i;
				if (DEBUG) printf("i: %d, a: %d, b: %d, c: %d\n", (buffer_i / 3) - 1, index_buffer[buffer_i - 3], index_buffer[buffer_i - 2], index_buffer[buffer_i - 1]);
			}
		}
	}
}

Mesh* SphericalMeshBuilder::GetMesh() {
	return &mesh;
}

void SphericalMeshBuilder::Print() {

	std::cout << "\nSpherical Mesh Builder:\n\n" <<
		"\tnum_lon: " << num_lon << std::endl <<
		"\tnum_lat: " << num_lat << std::endl <<
		"\tnum_depth: " << num_depth << std::endl <<

		"\tlon_step: " << lon_step << std::endl <<
		"\tlat_step: " << lat_step << std::endl <<

		"\tscale: " << scale << std::endl;

}