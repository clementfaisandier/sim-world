#include "spherical-mesh-builder.h"

SphericalMeshBuilder::SphericalMeshBuilder(float scale_min, float scale_max, unsigned int num_lon, unsigned int num_lat, unsigned int num_layers) {

	if (scale_min == 0 || scale_max < scale_min || num_lon == 0 || num_lat == 0 || num_layers == 0) {
		std::cout << "Error: SphericalMeshBuilder: Invalid Input\n";
		std::exit(1);
	}

	InitFields(scale_min, scale_max, num_lon, num_lat, num_layers);
	InitSurfaceMesh();
	InitAthmosphericMesh();

	DefineSurfaceVertices();
	DefineSurfaceIndices();

	DefineAthmosphericMesh();
	DefineAthmosphericMeshGraphicVertices();
	DefineAthmosphericGraphicIndices();
}

void SphericalMeshBuilder::InitFields(float scale_min, float scale_max, unsigned int num_lon, unsigned int num_lat, unsigned int num_layers) {

	this->num_lon = num_lon;
	this->num_lat = num_lat;
	this->num_layers = num_layers;

	this->lon_step = glm::radians(360.0 / num_lon);
	this->lat_step = glm::radians(180.0 / num_lat);
	this->layer_step = (scale_max - scale_min) * 1.0 / num_layers;

	this->scale_min = scale_min;
	this->scale_max = scale_max;
}

void SphericalMeshBuilder::InitSurfaceMesh() {

	surface_mesh.primary_count = num_lon;
	surface_mesh.secondary_count = num_lat;
	surface_mesh.tertiary_count = 1;

	surface_mesh.vertex_buffer_count = (num_lon * (num_lat - 1) + 2);
	surface_mesh.index_buffer_count = (num_lon * (num_lat - 1) * 2); // only applicable to current index setup

	surface_mesh.vertex_buffer_size = surface_mesh.vertex_buffer_count * surface_mesh.vertex_size;
	surface_mesh.index_buffer_size = sizeof(*surface_mesh.index_buffer) * N_PRIMITIVE_COMPONENTS * surface_mesh.index_buffer_count;

	surface_mesh.vertex_buffer = (float*)malloc(surface_mesh.vertex_buffer_size);
	surface_mesh.index_buffer = (unsigned int*)malloc(surface_mesh.index_buffer_size);

	if (surface_mesh.vertex_buffer == NULL || surface_mesh.index_buffer == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate vertex/index buffer.\nVB: %u, IB: %u\n", &surface_mesh.vertex_buffer, &surface_mesh.index_buffer);
		exit(errno);
	}
}

void SphericalMeshBuilder::InitAthmosphericMesh() {

	athmospheric_mesh.primary_count = num_lon;
	athmospheric_mesh.secondary_count = num_lat;
	athmospheric_mesh.tertiary_count = num_layers;

	athmospheric_mesh.total_count = (num_lon * (num_lat - 1) + 2) * num_layers;
	athmospheric_mesh.total_size = athmospheric_mesh.total_count * sizeof(ComputeMesh::Cell);

	athmospheric_mesh.mesh = (ComputeMesh::Cell*) malloc(athmospheric_mesh.total_size);
	
	athmospheric_mesh.display_volumes = new Mesh();
	Mesh* display_volumes = athmospheric_mesh.display_volumes;

	display_volumes->primary_count = num_lon;
	display_volumes->secondary_count = num_lat;
	display_volumes->secondary_count = num_layers;

	display_volumes->vertex_buffer_count = (num_lon * (num_lat - 1) + 2) * num_layers;
	display_volumes->index_buffer_count = (num_lon * (num_lat - 1) * 2) * num_layers;
	
	display_volumes->vertex_buffer_size = display_volumes->vertex_buffer_count * display_volumes->vertex_size;
	display_volumes->index_buffer_size = sizeof(*display_volumes->index_buffer) * N_PRIMITIVE_COMPONENTS * display_volumes->index_buffer_count;

	display_volumes->vertex_buffer = (float*)malloc(display_volumes->vertex_buffer_size);
	display_volumes->index_buffer = (unsigned int*)malloc(display_volumes->index_buffer_size);
}

void SphericalMeshBuilder::DefineSurfaceVertices() {

	int vertex_i = 0;

	float* vertex_buffer = surface_mesh.vertex_buffer;

	// north pole
	vertex_buffer[vertex_i++] = 0.0; // x
	vertex_buffer[vertex_i++] = 0.0; // y
	vertex_buffer[vertex_i++] = scale_min; // z

	for (int lat_i = 1; lat_i < num_lat; lat_i++) { // defines non-pole vertices

		float lat = H_PI - (lat_step * lat_i);

		for (int lon_i = 0; lon_i < num_lon; lon_i++) {

			float lon = lon_step * lon_i;

			vertex_buffer[vertex_i++] = cos(lat) * sin(lon) * scale_min; // x
			vertex_buffer[vertex_i++] = cos(lat) * cos(lon) * scale_min; // y
			vertex_buffer[vertex_i++] = sin(lat) * scale_min; // z
		}
	}

	// south pole
	vertex_buffer[vertex_i++] = 0.0; // x
	vertex_buffer[vertex_i++] = 0.0; // y
	vertex_buffer[vertex_i++] = -scale_min; // z
}

void SphericalMeshBuilder::DefineSurfaceIndices() {

	unsigned int* index_buffer = surface_mesh.index_buffer;

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

void SphericalMeshBuilder::DefineAthmosphericMesh() {

	for (int i = 0; i < athmospheric_mesh.total_count; i++) {
		athmospheric_mesh.mesh[i].velocity = glm::vec3(0, 0, 0);
		athmospheric_mesh.mesh[i].pressure = 1;
		athmospheric_mesh.mesh[i].density = 1;
	}
}

void SphericalMeshBuilder::DefineAthmosphericMeshGraphicVertices() {

	float* vertex_buffer = athmospheric_mesh.display_volumes->vertex_buffer;

	int vertex_i = 0;

	for (int layer_i = 0; layer_i < num_layers; layer_i++) {

		float layer_scale = scale_min + (layer_i * layer_step);

		// north pole
		vertex_buffer[vertex_i++] = 0.0; // x
		vertex_buffer[vertex_i++] = 0.0; // y
		vertex_buffer[vertex_i++] = layer_scale; // z

		for (int lat_i = 1; lat_i < num_lat; lat_i++) { // defines non-pole vertices

			float lat = H_PI - (lat_step * lat_i);

			for (int lon_i = 0; lon_i < num_lon; lon_i++) {

				float lon = lon_step * lon_i;

				vertex_buffer[vertex_i++] = cos(lat) * sin(lon) * layer_scale; // x
				vertex_buffer[vertex_i++] = cos(lat) * cos(lon) * layer_scale; // y
				vertex_buffer[vertex_i++] = sin(lat) * layer_scale; // z
			}
		}

		// south pole
		vertex_buffer[vertex_i++] = 0.0; // x
		vertex_buffer[vertex_i++] = 0.0; // y
		vertex_buffer[vertex_i++] = -layer_scale; // z
	}
}

void SphericalMeshBuilder::DefineAthmosphericGraphicIndices() {
	unsigned int* index_buffer = athmospheric_mesh.display_volumes->index_buffer;

	unsigned int buffer_i = 0;

	unsigned int i_offset_per_layer = athmospheric_mesh.display_volumes->index_buffer_count / athmospheric_mesh.display_volumes->tertiary_count;

	for (int layer_i = 0; layer_i < num_layers; layer_i++) {

		unsigned int layer_vertex_i_offset = i_offset_per_layer * layer_i;
		unsigned int top_vertex_i = 0;
		unsigned int bot_vertex_i = 1;

		for (int lat_i = 0; lat_i < num_lat; lat_i++) {

			unsigned int initial_top_vertex = top_vertex_i;
			unsigned int initial_bot_vertex = bot_vertex_i;

			for (int lon_i = 1; lon_i <= num_lon; lon_i++) {

				if (lat_i == 0) {
					index_buffer[buffer_i++] = layer_vertex_i_offset + top_vertex_i;
					index_buffer[buffer_i++] = lon_i == num_lon ? layer_vertex_i_offset + initial_bot_vertex : layer_vertex_i_offset + bot_vertex_i + 1;
					index_buffer[buffer_i++] = layer_vertex_i_offset + bot_vertex_i++;

					if (lon_i == num_lon)
						top_vertex_i++;

					if (DEBUG) printf("i: %d, a: %d, b: %d, c: %d\n", (buffer_i / 3) - 1, index_buffer[buffer_i - 3], index_buffer[buffer_i - 2], index_buffer[buffer_i - 1]);
				}

				else if (lat_i != num_lat - 1) {

					unsigned int top_left = layer_vertex_i_offset + top_vertex_i++;
					unsigned int top_right = lon_i == num_lon ? layer_vertex_i_offset + initial_top_vertex : layer_vertex_i_offset + top_vertex_i; // this longitude trick takes care of wrapping
					unsigned int bot_left = layer_vertex_i_offset + bot_vertex_i++;
					unsigned int bot_right = lon_i == num_lon ? layer_vertex_i_offset + initial_bot_vertex : layer_vertex_i_offset + bot_vertex_i;

					index_buffer[buffer_i++] = bot_left;
					index_buffer[buffer_i++] = top_left;
					index_buffer[buffer_i++] = bot_right;

					index_buffer[buffer_i++] = bot_right;
					index_buffer[buffer_i++] = top_left;
					index_buffer[buffer_i++] = top_right;

					if (DEBUG) printf("i: %d, a: %d, b: %d, c: %d, d: %d\n", (buffer_i / 3) - 2, top_left, top_right, bot_left, bot_right);
				}

				else {
					index_buffer[buffer_i++] = layer_vertex_i_offset + bot_vertex_i;
					index_buffer[buffer_i++] = layer_vertex_i_offset + top_vertex_i++;
					index_buffer[buffer_i++] = lon_i == num_lon ? layer_vertex_i_offset + initial_top_vertex : layer_vertex_i_offset + top_vertex_i;
					if (DEBUG) printf("i: %d, a: %d, b: %d, c: %d\n", (buffer_i / 3) - 1, index_buffer[buffer_i - 3], index_buffer[buffer_i - 2], index_buffer[buffer_i - 1]);
				}
			}
		}
	}
}

Mesh* SphericalMeshBuilder::GetSurfaceMesh() {
	return &surface_mesh;
}

ComputeMesh* SphericalMeshBuilder::GetComputeMesh() {
	return &athmospheric_mesh;
}

void SphericalMeshBuilder::Print() {

	std::cout << "\nSpherical Mesh Builder:\n\n" <<
		"\tnum_lon: " << num_lon << std::endl <<
		"\tnum_lat: " << num_lat << std::endl <<
		"\tnum_depth: " << num_layers << std::endl <<

		"\tlon_step: " << lon_step << std::endl <<
		"\tlat_step: " << lat_step << std::endl <<

		"\tscale: " << scale_min << std::endl;

}