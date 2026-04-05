#include "GridMeshBuilder.h"


/*
    Creating a grid mesh with the specified number of cells.
    Note that the resulting grid is scaled within [-1, 1] in all three axes.
*/
GridMeshBuilder::GridMeshBuilder(
        unsigned int num_x,
        unsigned int num_y,
        unsigned int num_z
) {

    this->num_x = num_x;
    this->num_y = num_y;
    this->num_z = num_z;

    float range = 2.0;

    this->x_step = range / ( num_x - 1);
    this->y_step = range / ( num_y - 1);
    this->z_step = range / ( num_z - 1);
}


GridComputeMesh* GridMeshBuilder::getComputeMesh()
{
    GridComputeMesh* mesh = new GridComputeMesh();
    mesh->num_x = num_x;
    mesh->num_x = num_y;
    mesh->num_x = num_z;
    mesh->compute_buffer_count = num_x * num_y * num_z;
    mesh->compute_buffer_size = sizeof(GridComputeMesh::Cell) * mesh->compute_buffer_count;
    mesh->compute_buffer = new GridComputeMesh::Cell[mesh->compute_buffer_count];

    unsigned int cbi = 0;

    for (int i = 0; i < num_x; i++) {
        for (int j = 0; j < num_y; j++) {
            for (int k = 0; k < num_z; k++) {
                mesh->compute_buffer[cbi].velocity = glm::vec3(0.0);
                mesh->compute_buffer[cbi].pressure = 1.0;
                mesh->compute_buffer[cbi].density  = 1.0;
                cbi++;
            }
        }
    }

    return mesh;
}



/*
    Returns a mesh with a cube defined for each cell.
    Each cube is defined with two triangles to a face.
    6 faces * 2 triangles * 3 vertices = 36 vertices per cube.
    6 faces * 2 triangles = 12 triangles per cube.

    Cubes are defined from the (-1, -1, -1) cell first to the
    (1, 1, 1) cell incrementing first in the x, then the y, then
    the z axis.

    Assumes face culling is clockwise.

    NOTE: these cubes do not share any vertices, which could
    be optimized in the future. However this setup is easier
    to implement and allows for margin between the cubes which
    could be pretty.

*/
GridGraphicsMesh* GridMeshBuilder::getVolumeMesh()
{
    const unsigned int k_vertices_per_cube = 36;
    const unsigned int k_triangles_per_cube = 36;

    GridGraphicsMesh* mesh = new GridGraphicsMesh();
    mesh->num_x = num_x;
    mesh->num_y = num_y;
    mesh->num_z = num_z;
    mesh->vertex_buffer_vertex_count = num_x * num_y * num_z * k_vertices_per_cube;
    mesh->index_buffer_triangle_count = num_x * num_y * num_z * k_triangles_per_cube;
    mesh->vertex_buffer_length = mesh->vertex_buffer_vertex_count * N_ATTR_P_VERTEX;
    mesh->index_buffer_length = mesh->index_buffer_triangle_count * N_VERTEX_P_PRIMITIVE;
    mesh->vertex_buffer_size = mesh->vertex_buffer_length * sizeof(mesh->vertex_buffer);
    mesh->index_buffer_size = mesh->index_buffer_length * sizeof(mesh->index_buffer);
    mesh->vertex_buffer = new float[mesh->vertex_buffer_length];
    mesh->index_buffer = new unsigned int[mesh->index_buffer_length];

    float* vertex_buffer = mesh->vertex_buffer;
    for (int i = 0; i < num_x; i++) {
        for (int j = 0; j < num_y; j++) {
            for (int k = 0; k < num_z; k++) {
                vertex_buffer = this->generateVolumeVertexAttributes(
                    vertex_buffer,
                    (i * x_step - 1),
                    (j * y_step - 1),
                    (k * z_step - 1)
                );
            }
        }
    }

    unsigned int starting_index = 0;
    unsigned int* index_buffer = mesh->index_buffer;
    for (int i = 0; i < num_x; i++) {
        for (int j = 0; j < num_y; j++) {
            for (int k = 0; k < num_z; k++) {
                index_buffer = this->generateVolumeIndexAttributes(
                    index_buffer,
                    starting_index
                );
                starting_index += k_vertices_per_cube;
            }
        }
    }

    return mesh;
}

/*
    Defines a cube who's center is the given coordinate in the provided buffer.
    Returns the address immediatly ater this newly defined space in memory:
        ie. the address for the buffer's next volume vertex definitions.
    
    The width, height, and depth of the cube are defined as the half
    of the x_step, y_step, and z_step variables respectively.

    Defines the vertices in the following order:

    ^ y
    |
    o----> x
     `.
       `> z

    2==========3
    |`.        |`.
    |  `6======+==`7
    |   |      |   |
    0===+------1   |
     `. |       `. |
       `4=========`5
    
    Where the face defined by vertices 1,2,3,4 is behind the face defined by vertices 5,6,7,8.
        Meaning vertices 1,2,3,4 have z values less than those of 5,6,7,8.
    
    Edges are axis-aligned.
*/
float* GridMeshBuilder::generateVolumeVertexAttributes(
        float* buffer,
        float x,
        float y,
        float z
) {
    const float x_offset = x_step / 2;
    const float y_offset = y_step / 2;
    const float z_offset = z_step / 2;
    // Vertex 0
    buffer[0] = x - x_offset;
    buffer[1] = y - y_offset;
    buffer[2] = z - z_offset;
    buffer += 3;
    // Vertex 1
    buffer[0] = x + x_offset;
    buffer[1] = y - y_offset;
    buffer[2] = z - z_offset;
    buffer += 3;
    // Vertex 2
    buffer[0] = x - x_offset;
    buffer[1] = y + y_offset;
    buffer[2] = z - z_offset;
    buffer += 3;
    // Vertex 3
    buffer[0] = x + x_offset;
    buffer[1] = y + y_offset;
    buffer[2] = z - z_offset;
    buffer += 3;
    // Vertex 4
    buffer[0] = x - x_offset;
    buffer[1] = y - y_offset;
    buffer[2] = z + z_offset;
    buffer += 3;
    // Vertex 5
    buffer[0] = x + x_offset;
    buffer[1] = y - y_offset;
    buffer[2] = z + z_offset;
    buffer += 3;
    // Vertex 6
    buffer[0] = x - x_offset;
    buffer[1] = y + y_offset;
    buffer[2] = z + z_offset;
    buffer += 3;
    // Vertex 7
    buffer[0] = x + x_offset;
    buffer[1] = y + y_offset;
    buffer[2] = z + z_offset;
    buffer += 3;
    return buffer;
}

/*
    Defines the indexes to produce a cube in the provided buffer. Indexes are
        produced for vertices generated by the generateVolumeVertexAttributes
        function. Indexes are generated assuming a GL_TRIANGLES primitive
        rendering mode and a clockwise-as-front face culling.

    Returns the address immediatly ater this newly defined space in memory:
        ie. the address for the buffer's next volume index definitions.

    Defines the vertices in the following order: a-z then A-J.
    The diagram below shows a cube, deconstructed in it's faces (squares),
        which themselves are split in triangles (the rendering primitive).

    ^ y
    |
    o----> x
     `.
       `> z

    Vertex Indexes

    2==========3
    |`.        |`.
    |  `6======+==`7
    |   |      |   |
    0===+------1   |
     `. |       `. |
       `4=========`5

    Index Order

        i,j--------k
         `.`'~~~~~..`.
           `h--------``g,l

           c,d--------f 
            |`~.       |
    A,B     |   `~.    |       m
    |`.     |      `~. |       |`.
    |\ `y   a---------`b,e     |  `o,p
    | \ |                      | / |
    D  \|       x---------.u,v n,q |
     `. |       |       .~'|    `. |
       `z,C     |    .~'   |      `r
                | .~'      |
                t'w-------`s

                E---------.G,H
                 `.   .~'   `.
                   `F'I------`J
    
    Faces abc,def are behind faces stu,vwx.

    TODO: I now realize there is a much simpler deconstruction
        of the square by first flattening the square as such:
    
        +---+
        |   |
        +---++---++---++---+
        |   ||   ||   ||   |
        +---++---++---++---+
                       |   |
                       +---+
    
        And with the square now a strip that can be split
        into a triangle split, it may be better to use the
        GL_TRIANGLE_STRIP primitive rendering mode.

        NOTE: This is effectively what I tried to do manually
        but I did make a mistake in the pattern while switching
        from the back face to the top face and the top to the
        right face.

*/
unsigned int* GridMeshBuilder::generateVolumeIndexAttributes(
        unsigned int* buffer,
        unsigned int starting_index
) {
    // Back Face
    // abc
    unsigned int i = 0;
    buffer[i++] = starting_index + 0;
    buffer[i++] = starting_index + 1;
    buffer[i++] = starting_index + 2;
    // def
    buffer[i++] = starting_index + 2;
    buffer[i++] = starting_index + 1;
    buffer[i++] = starting_index + 3;
    // Top Face
    // ghi
    buffer[i++] = starting_index + 7;
    buffer[i++] = starting_index + 6;
    buffer[i++] = starting_index + 2;
    // jkl
    buffer[i++] = starting_index + 2;
    buffer[i++] = starting_index + 3;
    buffer[i++] = starting_index + 7;
    // Right Face
    // mno
    buffer[i++] = starting_index + 3;
    buffer[i++] = starting_index + 1;
    buffer[i++] = starting_index + 7;
    // pqr
    buffer[i++] = starting_index + 7;
    buffer[i++] = starting_index + 1;
    buffer[i++] = starting_index + 5;
    // Front Face
    // stu
    buffer[i++] = starting_index + 5;
    buffer[i++] = starting_index + 4;
    buffer[i++] = starting_index + 7;
    // vwx
    buffer[i++] = starting_index + 7;
    buffer[i++] = starting_index + 4;
    buffer[i++] = starting_index + 6;
    // Left Face
    // yzA
    buffer[i++] = starting_index + 6;
    buffer[i++] = starting_index + 4;
    buffer[i++] = starting_index + 2;
    // BCD
    buffer[i++] = starting_index + 2;
    buffer[i++] = starting_index + 4;
    buffer[i++] = starting_index + 0;
    // Bottom Face
    // EFG
    buffer[i++] = starting_index + 0;
    buffer[i++] = starting_index + 4;
    buffer[i++] = starting_index + 1;
    // HIJ
    buffer[i++] = starting_index + 1;
    buffer[i++] = starting_index + 4;
    buffer[i++] = starting_index + 5;

    return buffer + i;
}
