#pragma once

#include "glm/vec3.hpp"

#define N_ATTR_P_VERTEX 3
#define N_VERTEX_P_PRIMITIVE 3

struct GridGraphicsMesh {

    float* vertex_buffer = nullptr;
    unsigned int* index_buffer = nullptr;

    // The number of bytes each buffer uses.
    unsigned int vertex_buffer_size = 0;
    unsigned int index_buffer_size = 0;

    // The number of floats in the buffer.
    unsigned int vertex_buffer_length = 0;
    unsigned int index_buffer_length = 0;

    // The number of vertices/triangles in the buffers.
    unsigned int vertex_buffer_vertex_count = 0;
    unsigned int index_buffer_triangle_count = 0;

    unsigned int num_x = 0;
    unsigned int num_y = 0;
    unsigned int num_z = 0;

    ~GridGraphicsMesh()
    {
        delete[] vertex_buffer;
        delete[] index_buffer;
    }
};

struct GridComputeMesh {


    struct Cell {
        glm::vec3 velocity;
        float pressure;
        float density;
    };

    Cell* compute_buffer = nullptr;

    unsigned int compute_buffer_size = 0;
    unsigned int compute_buffer_count = 0;

    unsigned int num_x = 0;
    unsigned int num_y = 0;
    unsigned int num_z = 0;
};



class GridMeshBuilder {

private:

    unsigned int num_x = 0;
    unsigned int num_y = 0;
    unsigned int num_z = 0;

    float x_step;
    float y_step;
    float z_step;

    float* generateVolumeVertexAttributes(
        float* buffer,
        float x,
        float y,
        float z 
    );

    unsigned int* generateVolumeIndexAttributes(
        unsigned int* buffer,
        unsigned int starting_index
    );

public:

    GridMeshBuilder(
        unsigned int num_x,
        unsigned int num_y,
        unsigned int num_z
    );

    GridComputeMesh* getComputeMesh();

    GridGraphicsMesh* getVolumeMesh();

};


