#include "pch.h"

#include "spherical-tensor.h"

TEST(SphericalTensorTest, Handles3_3) {

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

    SphericalTensor tensor = SphericalTensor(3, 3);

    ASSERT_EQ(tensor.GetVertexBuffer(), positions) << "SphericalTensor vertex buffer incorrect for (3, 3).";
    ASSERT_EQ(tensor.GetIndexBuffer(), indices) << "SphericalTensor index buffer incorrect for (3, 3).";
}