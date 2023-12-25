#include "pch.h"

#include "spherical-mesh-builder.h"

TEST(SphericalMeshTest, Handles3_2) {

    float positions[] = { // vertices of a square/rectangle
        0.0, 0.0, 1.0,
        0.0, 1.0, -0.0,
        0.866025, -0.5, -0.0,
        -0.866025, -0.5, -0.0,
        0.0, 0.0, -1
    };

    unsigned int indices[] = { // index buffer
        0, 1, 2,
        0, 2, 3,
        0, 3, 1,
        4, 1, 2,
        4, 2, 3,
        4, 3, 1
    };

    SphericalMesh mesh = SphericalMesh(3, 2);

    float* pos = mesh.GetVertexBuffer();
    unsigned int* ind = mesh.GetIndexBuffer();

    unsigned int pos_count = 5;
    unsigned int ind_count = 6;

    for (int i = 0; i < pos_count; i++) {
        for (int j = 0; j < 3; j++) {
            unsigned int index = (i * 3) + j;
            EXPECT_EQ(pos[index], positions [index]);
        }
    }

    for (int i = 0; i < ind_count; i++) {
        for (int j = 0; j < 3; j++) {
            unsigned int index = (i * 3) + j;
            EXPECT_EQ(ind[index], indices[index]);
        }
    }
}