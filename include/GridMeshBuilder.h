#pragma once

#include "glm/glm.hpp"
#include <iostream>
#include <cstring>
#include <random>

// coordinates (xyz) + colors (rgba)
#define N_ATTR_P_VERTEX_GRID 7
#define N_VERTEX_P_PRIMITIVE 3


inline glm::vec4 color_pallet[8] = {
    glm::vec4(244/255.0f,  67/255.0f,  54/255.0f, 0.01f),
    glm::vec4(103/255.0f,  58/255.0f, 183/255.0f, 0.01f),
    glm::vec4( 33/255.0f, 150/255.0f, 243/255.0f, 0.01f),
    glm::vec4( 76/255.0f, 175/255.0f,  80/255.0f, 0.01f),
    glm::vec4(255/255.0f, 152/255.0f,   0/255.0f, 0.01f),
    glm::vec4( 96/255.0f, 125/255.0f, 139/255.0f, 0.01f),
    glm::vec4(  0/255.0f, 188/255.0f, 212/255.0f, 0.01f),
    glm::vec4(233/255.0f,  30/255.0f,  99/255.0f, 0.01f)
};

struct GridGraphicsMesh {

    // Interweaved Vertex Buffer
    // 3 floats for x y z position
    // 4 floats for color: rgba
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

    glm::vec3 dimension = glm::vec3(0.0f);

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

    glm::vec3 dimension = glm::vec3(0.0f);
};



class GridMeshBuilder {

private:

    glm::vec3 dimension = glm::vec3(0.0f);

    float* generateVolumeVertexAttributes(
        float* buffer,
        glm::vec3 location,
        glm::vec3 width,
        glm::vec4 color
    );

    unsigned int* generateVolumeIndexAttributes(
        unsigned int* buffer,
        unsigned int starting_index
    );

public:

    GridMeshBuilder(
        glm::vec3 dimension
    );

    GridComputeMesh* getComputeMesh();

    GridGraphicsMesh* getVolumeMesh();

};


