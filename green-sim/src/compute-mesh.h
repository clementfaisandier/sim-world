#pragma once
#include <stdlib.h>
#include <GL\glew.h>
#include <iostream>

#include <glm/vec3.hpp>



class ComputeMesh {

public:

	struct Cell {
	
		glm::vec3 velocity;
		float pressure;
		float density = 1;
	};

	Cell* mesh = nullptr;

	unsigned int total_size = 0;
	unsigned int total_count = 0;

	unsigned int primary_count = 0; // lon
	unsigned int secondary_count = 0; // lat
	unsigned int tertiary_count = 0; // depth

	ComputeMesh() {}
	~ComputeMesh() {
		if (mesh != nullptr) { free(mesh); mesh = nullptr; }
	}

	void Print() {
		std::cout << "\nComputeMesh:\n" << std::endl <<

			"\ttotal_size: " << total_size << std::endl <<
			"\ttotal_count: " << total_count << std::endl <<

			"\tprimary_count: " << primary_count << std::endl <<
			"\tsecondary_count: " << secondary_count << std::endl <<
			"\ttertiary_count: " << tertiary_count << std::endl << std::endl;

		std::cout << "Mesh:\n" << std::endl;

		for (int i = 0; i < total_count; i++) {

			std::cout << "\tMesh: " << i << std::endl;
			printf("\t\tVelocity: x,y,z: %f %f %f\n", mesh[i].velocity[0]);
			std::cout << "\t\tPresure: " << mesh[i].pressure << std::endl <<
				"\t\tDensity: " << mesh[i].density << std::endl;
		}
	}
};