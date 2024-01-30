#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 translation_matrix;
uniform ivec3 u_dimension;
uniform vec4 u_color;
uniform int u_object;

out vec4 p_color;

void main()
{
    gl_Position = translation_matrix * position;


    // set vertex color based on the object
    // u_object == 0 is the surface -> use the texture color
    // u_object == 1 is the wireframe -> apply color based on location

    /*
    if (u_object == 0) {
        p_color = vec4(0.5, 0.2, 0.2, 1.0);
    }
    else if (u_object == 1) {

        int num_lon = u_dimension.x;
        int num_lat = u_dimension.y;
        int num_layer = u_dimension.z;
        int num_volumes_per_layer = (num_lon * (num_lat-2) + 2);
    
        int volume_id = int(gl_VertexID / 4);

        float layer = volume_id / num_volumes_per_layer;

        int volume_id_in_layer = volume_id % num_volumes_per_layer;

        if (volume_id_in_layer == 0 || volume_id_in_layer == num_lon-1) {
            p_color = vec4(1.0, 1.0, 0.0, 1.0);
            return;
        }

        float lat = volume_id_in_layer / num_lon;
        float lon = volume_id_in_layer % num_lon;

        float r =  sin (20 * lon / num_lon) * 0.8;
        float g = cos (20 * lat / num_lat) * 0.8;
        float b =  1 - (layer / num_layer);
        float a = 0.15;
        */
        p_color = vec4(1, 1, 1, 0.5);


    //}
}