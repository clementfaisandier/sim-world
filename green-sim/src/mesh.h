#pragma once
#include <stdlib.h>
#include <GL\glew.h>
#include <iostream>

#define N_VERTEX_COMPONENTS 3
#define N_PRIMITIVE_COMPONENTS 3



class Mesh {

public:

	float* vertex_buffer = nullptr;
	unsigned int* index_buffer = nullptr;

	unsigned int vertex_buffer_size = 0;
	unsigned int index_buffer_size = 0;

	unsigned int vertex_buffer_count = 0;
	unsigned int index_buffer_count = 0; // num primitive draw calls

	unsigned int primary_count = 0; // lon
	unsigned int secondary_count = 0; // lat
	unsigned int tertiary_count = 0; // depth

	unsigned int vertex_num_components = N_VERTEX_COMPONENTS;
	unsigned int vertex_size = sizeof(*vertex_buffer) * N_VERTEX_COMPONENTS; // stride

	unsigned int primitive_num_components = N_PRIMITIVE_COMPONENTS;

	Mesh() {}
	~Mesh() {
		if (vertex_buffer != nullptr) free(vertex_buffer);
		if (index_buffer != nullptr) free(index_buffer);
	}

	void Print() {
		std::cout << "\n\nMesh:\n\tvertex_buffer_size: " << vertex_buffer_size << std::endl
			<< "\tindex_buffer_size: " << index_buffer_size << std::endl

			<< "\tvertex_buffer_count: " << vertex_buffer_count << std::endl
			<< "\tindex_buffer_count: " << index_buffer_count << std::endl

			<< "\tprimary_count: " << primary_count << std::endl
			<< "\tsecondary_count: " << secondary_count << std::endl
			<< "\ttertiary_count: " << tertiary_count << std::endl

			<< "\tvertex_num_components: " << vertex_num_components << std::endl
			<< "\tvertex_size: " << vertex_size << std::endl

			<< "\tprimitive_num_components: " << primitive_num_components << std::endl;


		std::cout << "\nVertex Buffer:\n" << std::endl;
		unsigned int v_index = 0;

		for (int i = 0; i < tertiary_count; i++) {
			std::cout << "\ttertiary_count: " << i << std::endl;

			std::cout << "\t\tsecondary_count: " << 0 << std::endl;
			printf("\t\t\tprimary_count: 0 x,y,z: %f, %f, %f,\n", vertex_buffer[v_index], vertex_buffer[v_index + 1], vertex_buffer[v_index + 2]); v_index += 3;

			for (int j = 1; j < secondary_count; j++) {
				std::cout << "\t\tsecondary_count: " << j << std::endl;

				for (int k = 0; k < primary_count; k++) {
					printf("\t\t\tprimary_count: %d x,y,z: %f, %f, %f,\n", k, vertex_buffer[v_index], vertex_buffer[v_index + 1], vertex_buffer[v_index + 2]); v_index += 3;
				}
			}

			std::cout << "\t\tsecondary_count: " << secondary_count << std::endl;
			printf("\t\t\tprimary_count: %d x,y,z: %f, %f, %f,\n", primary_count, vertex_buffer[v_index], vertex_buffer[v_index + 1], vertex_buffer[v_index + 2]); v_index += 3;
		}

		std::cout << "\nIndex Buffer:\n" << std::endl;

		unsigned int i_index = 0;

		for (int i = 0; i < index_buffer_count; i++) {
			printf("\ti: %d a,b,c: %d, %d, %d\n", i, index_buffer[i_index], index_buffer[i_index + 1], index_buffer[i_index + 2]); i_index += 3;
		}
	}
};