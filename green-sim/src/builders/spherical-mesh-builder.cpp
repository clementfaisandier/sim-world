#include "spherical-mesh-builder.h"


// PUBLIC

SphericalMeshBuilder::SphericalMeshBuilder(unsigned int num_lon, unsigned int num_lat, unsigned int num_layers, float scale_min, float scale_max) {
	if (scale_min == 0 || scale_max < scale_min || num_lon == 0 || num_lat == 0 || num_layers == 0) {
		std::cout << "Error: SphericalMeshBuilder: Invalid Input\n";
		std::exit(1);
	}

	this->num_lon = num_lon;
	this->num_lat = num_lat;
	this->num_layers = num_layers;

	this->scale_min = scale_min;
	this->scale_max = scale_max;

	this->lon_step = glm::radians(360.0 / num_lon);
	this->lat_step = glm::radians(180.0 / num_lat);
	this->layer_step = (scale_max - scale_min) / num_layers;
}

SphericalGraphicsMesh* SphericalMeshBuilder::GetSurfaceMesh() {

	// Allocate memory for the mesh
	SphericalGraphicsMesh* mesh  = (SphericalGraphicsMesh*) malloc(sizeof(SphericalGraphicsMesh));
	if (mesh == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate mesh.\n");
		exit(errno);
	}

	// init mesh fields

	mesh->num_lon = num_lon;
	mesh->num_lat = num_lat;
	mesh->num_layers = 1;

	mesh->vertex_buffer_count = (num_lon * (num_lat - 1) + 2);
	mesh->index_buffer_count = (num_lon * (num_lat - 1) * 2);

	mesh->vertex_buffer_size = sizeof (*mesh->vertex_buffer) * mesh->vertex_buffer_count * N_ATTR_P_VERTEX;
	mesh->index_buffer_size = sizeof(*mesh->index_buffer) * mesh->index_buffer_count * N_VERTEX_P_PRIMITIVE;

	// allocate vertex and index buffer

	mesh->vertex_buffer = (float*)malloc(mesh->vertex_buffer_size);
	mesh->index_buffer = (unsigned int*)malloc(mesh->index_buffer_size);

	if (mesh->vertex_buffer == NULL || mesh->index_buffer == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate vertex/index buffer.\nVB: %u, IB: %u\n", &mesh->vertex_buffer, &mesh->index_buffer);
		exit(errno);
	}

	// define vertex and index buffer	

	DefineSurfaceVertexBuffer(mesh->vertex_buffer, scale_min);
	DefineSurfaceIndexBuffer(mesh->index_buffer);

	return mesh;
}

SphericalGraphicsMesh* SphericalMeshBuilder::GetAthmosphericMesh() {

	// Allocate memory for the mesh
	SphericalGraphicsMesh* mesh = (SphericalGraphicsMesh*)malloc(sizeof(SphericalGraphicsMesh));
	if (mesh == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate mesh.\n");
		exit(errno);
	}

	// init mesh fields

	mesh->num_lon = num_lon;
	mesh->num_lat = num_lat;
	mesh->num_layers = num_layers;

	unsigned int vertex_count_per_layer = (num_lon * (num_lat - 1) + 2) * 4;
	unsigned int index_count_per_layer = (num_lon * (num_lat - 1) + 2) * 4;
	mesh->vertex_buffer_count = vertex_count_per_layer * num_layers;
	mesh->index_buffer_count = index_count_per_layer * num_layers;

	mesh->vertex_buffer_size = sizeof(*mesh->vertex_buffer) * mesh->vertex_buffer_count * N_ATTR_P_VERTEX;
	mesh->index_buffer_size = sizeof(*mesh->index_buffer) * mesh->index_buffer_count * N_VERTEX_P_PRIMITIVE;

	// allocate vertex and index buffer

	mesh->vertex_buffer = (float*)malloc(mesh->vertex_buffer_size);
	mesh->index_buffer = (unsigned int*)malloc(mesh->index_buffer_size);

	if (mesh->vertex_buffer == NULL || mesh->index_buffer == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate vertex/index buffer.\nVB: %u, IB: %u\n", &mesh->vertex_buffer, &mesh->index_buffer);
		exit(errno);
	}

	// define vertex and index buffer	

	DefineAthmosphereVertexBuffer(mesh->vertex_buffer);
	DefineAthmosphereIndexBuffer(mesh->index_buffer);

	return mesh;
}

SphericalComputeMesh* SphericalMeshBuilder::GetComputeMesh() {

	// Allocate memory for the mesh

	SphericalComputeMesh* mesh = (SphericalComputeMesh*)malloc(sizeof(SphericalComputeMesh));
	if (mesh == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate mesh.\n");
		exit(errno);
	}


	// init mesh fields

	mesh->num_lon = num_lon;
	mesh->num_lat = num_lat;
	mesh->num_layers = num_layers;

	mesh->compute_buffer_count = (num_lon * (num_lat - 1) + 2) * num_layers;

	mesh->compute_buffer_size = sizeof(*mesh->compute_buffer) * mesh->compute_buffer_count;

	// allocate compute buffer

	mesh->compute_buffer = (SphericalComputeMesh::Cell*)malloc(mesh->compute_buffer_size);

	if (mesh->compute_buffer == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate compute buffer.\n");
		exit(errno);
	}

	// define compute buffer

	DefineComputeBuffer(mesh->compute_buffer);

	return mesh;
}

// PRIVATE

int SphericalMeshBuilder::DefineSurfaceVertexBuffer(float* vertex_buffer, float scale) {

	unsigned int vbi = 0; // vertex buffer index

	// Define the top vertex
	vertex_buffer[vbi++] = 0.0f;
	vertex_buffer[vbi++] = scale;
	vertex_buffer[vbi++] = 0.0f;

	// Define middle vertices
	for (int i = 1; i < num_lat; i++) {
		for (int j = 0; j < num_lon; j++) {

			float lon = j * lon_step;
			float lat = i * lat_step;

			float x = scale * sin(lat) * cos(lon);
			float y = scale * cos(lat);
			float z = scale * sin(lat) * sin(lon);

			vertex_buffer[vbi++] = x;
			vertex_buffer[vbi++] = y;
			vertex_buffer[vbi++] = z;
		}
	}

	// Define the bottom vertex
	vertex_buffer[vbi++] = 0.0f;
	vertex_buffer[vbi++] = -scale;
	vertex_buffer[vbi++] = 0.0f;

	return vbi / N_ATTR_P_VERTEX; // TODO VERIFY
}

int SphericalMeshBuilder::DefineSurfaceIndexBuffer(unsigned int* index_buffer) {

	unsigned int ibi = 0; // index buffer index

	// Define the top triangle fan
	for (int i = 0; i < num_lon; i++) {
		index_buffer[ibi++] = 0;
		index_buffer[ibi++] = i + 1;
		index_buffer[ibi++] = (i + 1) % num_lon + 1;
	}

	// Define the middle triangle strip
	for (int i = 0; i < num_lat - 2; i++) {
		for (int j = 0; j < num_lon; j++) {

			unsigned int top_left = i * num_lon + j + 1;
			unsigned int bottom_left = (i + 1) * num_lon + j + 1;

			unsigned int top_right = i * num_lon + (j + 1) % num_lon + 1;
			unsigned int bottom_right = (i + 1) * num_lon + (j + 1) % num_lon + 1;

			index_buffer[ibi++] = top_left;
			index_buffer[ibi++] = bottom_left;
			index_buffer[ibi++] = top_right;

			index_buffer[ibi++] = top_right;
			index_buffer[ibi++] = bottom_left;
			index_buffer[ibi++] = bottom_right;


			/* Using CCW winding order, the triangles are defined as follows:
			+-----+
			|    /|
			|	/ |
			|  /  |
			| /   |
			|/    |
			+-----+
			*/
		}
	}

	// Define the bottom triangle fan
	for (int i = 0; i < num_lon; i++) {
		index_buffer[ibi++] = (num_lat - 2) * num_lon + (i + 1) % num_lon + 1;
		index_buffer[ibi++] = (num_lat - 2) * num_lon + i + 1;
		index_buffer[ibi++] = num_lon * (num_lat - 1) + 1;
	}

	return ibi / N_VERTEX_P_PRIMITIVE;
}

int SphericalMeshBuilder::DefineAthmosphereVertexBuffer(float* vertex_buffer) {

	unsigned int vbi = 0; // vertex buffer index

	for (int i = 0; i < num_layers; i++) {

		float scale = scale_min + 0.1 + i * layer_step;

		// Define the top component vertices
		DefineAthmosphereComponentVertices(vertex_buffer, &vbi, 0.0f, scale, 0.0f);

		// Define middle component vertices
		for (int j = 1; j < num_lat; j++) {
			for (int k = 0; k < num_lon; k++) {

				float lon = k * lon_step;
				float lat = j * lat_step;

				float x = scale * sin(lat) * cos(lon);
				float y = scale * cos(lat);
				float z = scale * sin(lat) * sin(lon);

				DefineAthmosphereComponentVertices(vertex_buffer, &vbi, x, y, z);
			}
		}

		// Define the bottom component vertices
		DefineAthmosphereComponentVertices(vertex_buffer, &vbi, 0.0f, -scale, 0.0f);
	}

	return vbi / N_ATTR_P_VERTEX; // TODO VERIFY
}

void SphericalMeshBuilder::DefineAthmosphereComponentVertices(float* vertex_buffer, unsigned int* vbi, float x, float y, float z) {

	float size = (scale_max - scale_min) / num_layers / 2;

	vertex_buffer[(*vbi)++] = x;
	vertex_buffer[(*vbi)++] = y;
	vertex_buffer[(*vbi)++] = z;

	vertex_buffer[(*vbi)++] = x+size;
	vertex_buffer[(*vbi)++] = y;
	vertex_buffer[(*vbi)++] = z;

	vertex_buffer[(*vbi)++] = x;
	vertex_buffer[(*vbi)++] = y+size;
	vertex_buffer[(*vbi)++] = z;

	vertex_buffer[(*vbi)++] = x;
	vertex_buffer[(*vbi)++] = y;
	vertex_buffer[(*vbi)++] = z+size;
}

int SphericalMeshBuilder::DefineAthmosphereIndexBuffer(unsigned int* index_buffer) {

	unsigned int ibi = 0; // index buffer index
	
	for (int i = 0; i < num_layers; i++) {

		unsigned int layer_offset = i * (num_lon * (num_lat - 1) + 2) * 4;

		// Define top volume
		DefineAthmosphericVolume(index_buffer, &ibi, layer_offset);
		

		// Define the middle volumes

		for (int j = 0; j < num_lat - 1; j++) {
			for (int k = 0; k < num_lon; k++) {

				unsigned int volume_offset = (layer_offset + 4) + (((j * num_lon) + k) * 4);

				DefineAthmosphericVolume(index_buffer, &ibi, volume_offset);

			}
		}

		// Define the bottom volume
		unsigned int volume_offset = (layer_offset + 4) + ((num_lat - 2) * num_lon * 4);

		DefineAthmosphericVolume(index_buffer, &ibi, layer_offset);

	}
	return ibi / N_VERTEX_P_PRIMITIVE;
}

void SphericalMeshBuilder::DefineAthmosphericVolume(unsigned int* index_buffer, unsigned int* ibi, unsigned int layer_offset) {
	index_buffer[(*ibi)++] = layer_offset;
	index_buffer[(*ibi)++] = layer_offset + 1;
	index_buffer[(*ibi)++] = layer_offset + 3;

	index_buffer[(*ibi)++] = layer_offset;
	index_buffer[(*ibi)++] = layer_offset + 2;
	index_buffer[(*ibi)++] = layer_offset + 1;

	index_buffer[(*ibi)++] = layer_offset;
	index_buffer[(*ibi)++] = layer_offset + 3;
	index_buffer[(*ibi)++] = layer_offset + 2;

	index_buffer[(*ibi)++] = layer_offset + 1;
	index_buffer[(*ibi)++] = layer_offset + 2;
	index_buffer[(*ibi)++] = layer_offset + 3;
}

int SphericalMeshBuilder::DefineComputeBuffer(SphericalComputeMesh::Cell* compute_buffer) {

	unsigned int cbi = 0; // compute buffer index
	
	for (int i = 0; i < num_layers; i++) {

		compute_buffer[cbi++] = SphericalComputeMesh::Cell{ glm::vec3(0.0, 0.0, 0.0), 0.0, 1.0 };

		for (int j = 1; j < num_lat-1; j++) {
			for (int k = 0; k < num_lon; k++) {

				compute_buffer[cbi++] = SphericalComputeMesh::Cell{ glm::vec3(0.0, 0.0, 0.0), 0.0, 1.0 };
			}
		}

		compute_buffer[cbi++] = SphericalComputeMesh::Cell{ glm::vec3(0.0, 0.0, 0.0), 0.0, 1.0 };

	}

	return cbi;
}

// Mesh Functions

void PrintSphericalGraphicsMesh(SphericalGraphicsMesh* mesh) {

	printf("\nSphericalGraphicsMesh:\n");
	printf("\tnum_lon: %u\n", mesh->num_lon);
	printf("\tnum_lat: %u\n", mesh->num_lat);
	printf("\tnum_layers: %u\n", mesh->num_layers);
	printf("\tvertex_buffer_count: %u\n", mesh->vertex_buffer_count);
	printf("\tindex_buffer_count: %u\n", mesh->index_buffer_count);
	printf("\tvertex_buffer_size: %u\n", mesh->vertex_buffer_size);
	printf("\tindex_buffer_size: %u\n", mesh->index_buffer_size);

	printf("\n\tvertex_buffer:\n");
	for (int i = 0; i < mesh->vertex_buffer_count; i++) {
		printf("\t\t%d: %f, %f, %f\n", i, mesh->vertex_buffer[i * N_ATTR_P_VERTEX], mesh->vertex_buffer[i * N_ATTR_P_VERTEX + 1], mesh->vertex_buffer[i * N_ATTR_P_VERTEX + 2]);
	}

	printf("\n\tindex_buffer:\n");
	for (int i = 0; i < mesh->index_buffer_count; i++) {
		printf("\t\t%d: %u, %u, %u\n", i, mesh->index_buffer[i * N_VERTEX_P_PRIMITIVE], mesh->index_buffer[i * N_VERTEX_P_PRIMITIVE + 1], mesh->index_buffer[i * N_VERTEX_P_PRIMITIVE + 2]);
	}
}

void PrintSphericalComputeMesh(SphericalComputeMesh* mesh) {

	printf("\nSphericalGraphicsMesh:\n");
	printf("\tnum_lon: %u\n", mesh->num_lon);
	printf("\tnum_lat: %u\n", mesh->num_lat);
	printf("\tnum_layers: %u\n", mesh->num_layers);
	printf("\tcompute_buffer_count: %u\n", mesh->compute_buffer_count);
	printf("\tcompute_buffer_size: %u\n", mesh->compute_buffer_size);

	printf("\n\tcompute_buffer:\n");
	for (int i = 0; i < mesh->compute_buffer_count; i++) {
		printf("\t\t%d: v: %f, %f, %f  p: %f  d: %f \n", i,
			mesh->compute_buffer[i].velocity[0], mesh->compute_buffer[i].velocity[1], mesh->compute_buffer[i].velocity[2],
			mesh->compute_buffer[i].pressure,
			mesh->compute_buffer[i].density);
	}
}