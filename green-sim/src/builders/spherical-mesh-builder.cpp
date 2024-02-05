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
	SphericalGraphicsMesh* mesh = (SphericalGraphicsMesh*)malloc(sizeof(SphericalGraphicsMesh));
	if (mesh == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate mesh.\n");
		exit(errno);
	}

	// init mesh fields

	mesh->num_lon = num_lon;
	mesh->num_lat = num_lat;
	mesh->num_layers = 1;

	mesh->vertex_buffer_count = (num_lon * (num_lat - 1) + 2);
	mesh->color_buffer_count = (num_lon * (num_lat - 1) + 2);
	mesh->index_buffer_count = (num_lon * (num_lat - 1) * 2);

	mesh->vertex_buffer_size = sizeof(*mesh->vertex_buffer) * mesh->vertex_buffer_count;
	mesh->color_buffer_size = sizeof(*mesh->color_buffer) * mesh->color_buffer_count;
	mesh->index_buffer_size = sizeof(*mesh->index_buffer) * mesh->index_buffer_count * N_VERTEX_P_PRIMITIVE;

	// allocate vertex and index buffer

	mesh->vertex_buffer = (glm::vec3*)malloc(mesh->vertex_buffer_size);
	mesh->color_buffer = (glm::vec4*)malloc(mesh->color_buffer_size);
	mesh->index_buffer = (unsigned int*)malloc(mesh->index_buffer_size);

	if (mesh->vertex_buffer == NULL || mesh->index_buffer == NULL || mesh->color_buffer == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate vertex/color/index buffer.\nVB: %u, CB: %u, IB: %u\n", &mesh->vertex_buffer, &mesh->color_buffer, &mesh->index_buffer);
		exit(errno);
	}

	// define vertex and index buffer	

	// could compare these returns with the count to verify the correct number of vertices and indices are defined
	// TODO: verify the return values OR should this be a unit test?
	DefineSurfaceVertexBuffer(mesh->vertex_buffer, scale_min);
	DefineSurfaceColorBuffer(mesh->color_buffer);
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
	mesh->color_buffer_count = vertex_count_per_layer * num_layers;
	mesh->index_buffer_count = index_count_per_layer * num_layers;

	mesh->vertex_buffer_size = sizeof(*mesh->vertex_buffer) * mesh->vertex_buffer_count;
	mesh->color_buffer_size = sizeof(*mesh->color_buffer) * mesh->color_buffer_count;
	mesh->index_buffer_size = sizeof(*mesh->index_buffer) * mesh->index_buffer_count * N_VERTEX_P_PRIMITIVE;

	// allocate vertex and index buffer

	mesh->vertex_buffer = (glm::vec3*)malloc(mesh->vertex_buffer_size);
	mesh->color_buffer = (glm::vec4*)malloc(mesh->color_buffer_size);
	mesh->index_buffer = (unsigned int*)malloc(mesh->index_buffer_size);

	if (mesh->vertex_buffer == NULL || mesh->index_buffer == NULL || mesh->color_buffer == NULL) {
		printf("ERROR: SphericalMeshBuilder: InitFields\nUnable to allocate vertex/color/index buffer.\nVB: %u, CB: %u, IB: %u\n", &mesh->vertex_buffer, &mesh->color_buffer, &mesh->index_buffer);
		exit(errno);
	}

	// define vertex and index buffer	

	// could compare these returns with the count to verify the correct number of vertices and indices are defined
	// TODO: verify the return values OR should this be a unit test?
	DefineAthmosphereVertexBuffer(mesh->vertex_buffer);
	DefineAthmosphereColorBuffer(mesh->color_buffer);
	DefineAthmosphereIndexBuffer(mesh->index_buffer);

	return mesh;
}

SphericalComputeMesh* SphericalMeshBuilder::GetComputeMesh() {

	// Allocate memory for the mesh

	SphericalComputeMesh* mesh = (SphericalComputeMesh*)malloc(sizeof(SphericalComputeMesh));
	if (mesh == NULL) {
		printf("ERROR: SphericalMeshBuilder: GetComputeMesh\nUnable to allocate compute mesh object.\n");
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

int SphericalMeshBuilder::DefineSurfaceVertexBuffer(glm::vec3* vertex_buffer, float scale) {

	unsigned int vbi = 0; // vertex buffer index

	// Define the top vertex
	vertex_buffer[vbi++] = glm::vec3(0.0f, scale, 0.0f);

	// Define middle vertices
	for (int i = 1; i < num_lat; i++) {
		for (int j = 0; j < num_lon; j++) {

			float lon = j * lon_step;
			float lat = i * lat_step;

			float x = scale * sin(lat) * cos(lon);
			float y = scale * cos(lat);
			float z = scale * sin(lat) * sin(lon);

			vertex_buffer[vbi++] = glm::vec3(x, y, z);
		}
	}

	// Define the bottom vertex
	vertex_buffer[vbi++] = glm::vec3(0.0f, -scale, 0.0f);


	return vbi;
}

int SphericalMeshBuilder::DefineSurfaceColorBuffer(glm::vec4* color_buffer) {

	unsigned int cbi = 0; // color buffer index

	glm::vec4 static_color = glm::vec4(0.2, 0.2, 0.6, 1.0);

	// Define the top vertex
	color_buffer[cbi++] = static_color;

	// Define middle vertices
	for (int i = 0; i < (num_lat - 1) * num_lon; i++) {

		color_buffer[cbi++] = static_color;
	}

	// Define the bottom vertex
	color_buffer[cbi++] = static_color;

	return cbi;
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


int SphericalMeshBuilder::DefineAthmosphereVertexBuffer(glm::vec3* vertex_buffer) {

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

	return vbi; // TODO VERIFY
}

void SphericalMeshBuilder::DefineAthmosphereComponentVertices(glm::vec3* vertex_buffer, unsigned int* vbi, float x, float y, float z) {

	float size = (scale_max - scale_min) / num_layers / 2;

	vertex_buffer[(*vbi)++] = glm::vec3(x, y, z);

	vertex_buffer[(*vbi)++] = glm::vec3(x + size, y, z);

	vertex_buffer[(*vbi)++] = glm::vec3(x, y + size, z);

	vertex_buffer[(*vbi)++] = glm::vec3(x, y, z + size);
}

int SphericalMeshBuilder::DefineAthmosphereColorBuffer(glm::vec4* color_buffer) {

	unsigned int cbi = 0; // color buffer index

	for (int i = 0; i < ((num_lat - 1) * num_lon + 2) * num_layers; i++) {
		DefineAthmosphereComponentColors(color_buffer, &cbi);
	}

	return cbi;
}

void SphericalMeshBuilder::DefineAthmosphereComponentColors(glm::vec4* color_buffer, unsigned int* cbi) {

	glm::vec4 static_color = glm::vec4(1.0, 0.0, 0.0, 1.0);

	color_buffer[(*cbi)++] = static_color;
	color_buffer[(*cbi)++] = static_color;
	color_buffer[(*cbi)++] = static_color;
	color_buffer[(*cbi)++] = static_color;

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
		unsigned int volume_offset = (layer_offset + 4) + ((num_lat - 1) * num_lon * 4);

		DefineAthmosphericVolume(index_buffer, &ibi, volume_offset);

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

	SphericalComputeMesh::Cell cell { glm::vec3(0.0, 0.5, 1.0), 1.0, 1.0 };

	for (int i = 0; i < num_layers; i++) {

		compute_buffer[cbi++] = cell;

		for (int j = 1; j < num_lat; j++) {
			for (int k = 0; k < num_lon; k++) {

				compute_buffer[cbi++] = cell;
			}
		}

		compute_buffer[cbi++] = cell;

	}

	return cbi;
}

// Mesh Functions

void SphericalGraphicsMesh::Print() {

	PrintMeta();

	printf("\n\tvertex_buffer:\n");
	for (int i = 0; i < vertex_buffer_count; i++) {
		glm::vec3 vertex = vertex_buffer[i];
		printf("\t\t%d: %f, %f, %f\n", i, vertex[0], vertex[1], vertex[2]);
	}

	printf("\n\tcolor_buffer:\n");
	for (int i = 0; i < color_buffer_count; i++) {
		glm::vec4 color = color_buffer[i];
		printf("\t\t%d: %f, %f, %f, %f\n", i, color[0], color[1], color[2], color[3]);
	}

	printf("\n\tindex_buffer:\n");
	for (int i = 0; i < index_buffer_count; i++) {
		printf("\t\t%d: %u, %u, %u\n", i, index_buffer[i * N_VERTEX_P_PRIMITIVE], index_buffer[i * N_VERTEX_P_PRIMITIVE + 1], index_buffer[i * N_VERTEX_P_PRIMITIVE + 2]);
	}
}

void SphericalGraphicsMesh::PrintMeta() {
	printf("\nSphericalGraphicsMesh:\n");
	printf("\tnum_lon: %u\n", num_lon);
	printf("\tnum_lat: %u\n", num_lat);
	printf("\tnum_layers: %u\n", num_layers);
	printf("\tvertex_buffer_count: %u\n", vertex_buffer_count);
	printf("\tcolor_buffer_count: %u\n", color_buffer_count);
	printf("\tindex_buffer_count: %u\n", index_buffer_count);
	printf("\tvertex_buffer_size: %u\n", vertex_buffer_size);
	printf("\tcolor_buffer_size: %u\n", color_buffer_size);
	printf("\tindex_buffer_size: %u\n", index_buffer_size);
}

void SphericalComputeMesh::Print() {

	PrintMeta();

	printf("\n\tcompute_buffer:\n");
	for (int i = 0; i < compute_buffer_count; i++) {
		printf("\t\t%d: v: %f, %f, %f  p: %f  d: %f \n", i,
			compute_buffer[i].velocity[0], compute_buffer[i].velocity[1], compute_buffer[i].velocity[2],
			compute_buffer[i].pressure,
			compute_buffer[i].density);
	}
}

void SphericalComputeMesh::PrintMeta() {
	printf("\nSphericalComputeMesh:\n");
	printf("\tnum_lon: %u\n", num_lon);
	printf("\tnum_lat: %u\n", num_lat);
	printf("\tnum_layers: %u\n", num_layers);
	printf("\tcompute_buffer_count: %u\n", compute_buffer_count);
	printf("\tcompute_buffer_size: %u\n", compute_buffer_size);
}