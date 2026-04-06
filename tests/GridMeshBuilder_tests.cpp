#include <doctest/doctest.h>
#include "GridMeshBuilder.h"
#include <glm/glm.hpp>

TEST_CASE("GridComputeMesh Generation") {
    unsigned int nx = 2, ny = 2, nz = 2;
    GridMeshBuilder builder(nx, ny, nz);
    auto* mesh = builder.getComputeMesh();

    SUBCASE("Buffer allocation and sizing") {
        REQUIRE(mesh != nullptr); 
        CHECK(mesh->compute_buffer_count == 8); // 2*2*2
        // Ensure the bug in your code (assigning all to num_x) is fixed/caught
        CHECK(mesh->num_x == 2);
        CHECK(mesh->num_y == 2);
        CHECK(mesh->num_z == 2);
    }

    SUBCASE("Buffer initialization values") {
        for (unsigned int i = 0; i < mesh->compute_buffer_count; ++i) {
            CHECK(mesh->compute_buffer[i].pressure == doctest::Approx(1.0f));
            CHECK(mesh->compute_buffer[i].velocity == glm::vec3(0.0f));
        }
    }

    delete[] mesh->compute_buffer;
    delete mesh;
}

TEST_CASE("GridGraphicsMesh Vertex Logic") {
    GridMeshBuilder builder(2, 2, 2); // 1.0 step size
    auto* mesh = builder.getVolumeMesh();

    SUBCASE("Vertex count math") {
        // 8 cubes * 36 vertices per cube
        CHECK(mesh->vertex_buffer_vertex_count == 8 * 36);
    }

    SUBCASE("First cube vertex bounds") {
        // The first cube is at (-1, -1, -1). 
        // With step 2.0 and 2 cells, offset is 1.0.
        // Vertex 0 should be x - offset = -1 - 1 = -2? 
        // Wait, check your offset logic: x_offset = x_step / 2.
        
        float v0_x = mesh->vertex_buffer[0];
        float v0_y = mesh->vertex_buffer[1];
        float v0_z = mesh->vertex_buffer[2];

        // i=0, j=0, k=0 results in center (-1, -1, -1)
        // offset is 1.0. So Vertex 0 is (-2, -2, -2).
        CHECK(v0_x == doctest::Approx(-2.0f));
    }

    // Clean up
    delete[] mesh->vertex_buffer;
    delete[] mesh->index_buffer;
    delete mesh;
}