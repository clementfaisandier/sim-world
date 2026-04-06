#include <doctest/doctest.h>
#include "GridMeshBuilder.h"
#include <glm/glm.hpp>

TEST_CASE("GridComputeMesh") {
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

    delete mesh;
}

TEST_CASE("GridGraphicsMesh VolumeMesh") {
    GridMeshBuilder builder(2, 2, 2); // 1.0 step size
    auto* mesh = builder.getVolumeMesh();

    SUBCASE("Vertex count math") {
        // 8 cubes * 8 vertices or corners per cube
        CHECK(mesh->vertex_buffer_vertex_count == 8 * 8);
    }

    SUBCASE("First Cube Vertices") {
        // The first cube is at (-1, -1, -1). 
        // With step 2.0 and 2 cells, offset is 1.0.
        // i=0, j=0, k=0 results in center (-1, -1, -1)
        // offset is 1.0. So Vertex 0 is (-2, -2, -2).
        CHECK(mesh->vertex_buffer[0] == doctest::Approx(-2.0f));
        CHECK(mesh->vertex_buffer[1] == doctest::Approx(-2.0f));
        CHECK(mesh->vertex_buffer[2] == doctest::Approx(-2.0f));
        CHECK(mesh->vertex_buffer[3] == doctest::Approx(0.0f));
        CHECK(mesh->vertex_buffer[4] == doctest::Approx(-2.0f));
        CHECK(mesh->vertex_buffer[5] == doctest::Approx(-2.0f));
        unsigned int vertex_y_index = 3 * 7;
        CHECK(mesh->vertex_buffer[vertex_y_index + 0] == doctest::Approx(0.0f));
        CHECK(mesh->vertex_buffer[vertex_y_index + 1] == doctest::Approx(0.0f));
        CHECK(mesh->vertex_buffer[vertex_y_index + 2] == doctest::Approx(0.0f));
    }

    SUBCASE("Last cube vertex bounds") {
        // 3 coordinates * 8 vertices * 7 cubes
        unsigned int last_cube_index = 3 * 8 * 7;
        // Last cube should be at (0,0,0) and (2,2,2)
        //  for first and last vertex
        CHECK(mesh->vertex_buffer[last_cube_index + 0] == doctest::Approx(0.0f));
        CHECK(mesh->vertex_buffer[last_cube_index + 1] == doctest::Approx(0.0f));
        CHECK(mesh->vertex_buffer[last_cube_index + 2] == doctest::Approx(0.0f));
        unsigned int vertex_y_index = last_cube_index + (3 * 7);
        CHECK(mesh->vertex_buffer[vertex_y_index + 0] == doctest::Approx(2.0f));
        CHECK(mesh->vertex_buffer[vertex_y_index + 1] == doctest::Approx(2.0f));
        CHECK(mesh->vertex_buffer[vertex_y_index + 2] == doctest::Approx(2.0f));
    }

    SUBCASE("First Cube Indexes") {
        // back face
        CHECK(mesh->index_buffer[0] == 0);
        CHECK(mesh->index_buffer[1] == 1);
        CHECK(mesh->index_buffer[2] == 2);
        // top face
        CHECK(mesh->index_buffer[6] == 7);
        CHECK(mesh->index_buffer[7] == 6);
        CHECK(mesh->index_buffer[8] == 2);
        // right face, 2nd triangle
        // 2 prior face *  with 2 triangles * with 3 primitives
        uint index = (2 * 2 * 3) + 3;
        CHECK(mesh->index_buffer[index + 0] == 7);
        CHECK(mesh->index_buffer[index + 1] == 1);
        CHECK(mesh->index_buffer[index + 2] == 5);
        // front face
        // 3 prior face *  with 2 triangles * with 3 primitives
        index = 3 * 2 * 3;
        CHECK(mesh->index_buffer[index + 0] == 5);
        CHECK(mesh->index_buffer[index + 1] == 4);
        CHECK(mesh->index_buffer[index + 2] == 7);
    }

    // Clean up
    delete mesh;
}