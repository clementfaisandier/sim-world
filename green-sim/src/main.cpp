#include "main.h"

#define PI 3.14159265358979323846


int main(void)
{
    // Initialize the library
    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(GLFWErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context the current
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    // Error output and callbacks
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLErrorCallback, 0);

    // face culling optimization -> can lead to invisible triangles if the index buffer defines the trianges in a counter-clockwise fashion
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    // blending for alpha channel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);

    std::cout << glGetString(GL_VERSION) << std::endl;



    // MESHES AND VAOS ---------------------------------------

    int lon = 90;
    int lat = 45;
    int layers = 10;
    int scale_min = 1.0;
    int scale_max = 2.0;
    
    SphericalMeshBuilder mesh_builder = SphericalMeshBuilder(lon, lat, layers, scale_min, scale_max);

    SphericalGraphicsMesh* surface_mesh = mesh_builder.GetSurfaceMesh();
    SphericalGraphicsMesh* athmospheric_mesh = mesh_builder.GetAthmosphericMesh();

    //PrintSphericalGraphicsMesh(surface_mesh);
    //PrintSphericalGraphicsMesh(athmospheric_mesh);

    float* surface_positions = surface_mesh->vertex_buffer;
    unsigned int* surface_indices = surface_mesh->index_buffer;

    float* athmospheric_positions = athmospheric_mesh->vertex_buffer;
    unsigned int* athmospheric_indices = athmospheric_mesh->index_buffer;

    // create VAO
    unsigned int surface_vao;
    glGenVertexArrays(1, &surface_vao);
    glBindVertexArray(surface_vao);

    // vertex buffer
    unsigned int vertexBuffer; // buffer id
    glGenBuffers(1, &vertexBuffer); // generate the actual buffer in memory
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // tell opengl how to handle stride and how to navigate data
    glBufferData(GL_ARRAY_BUFFER, surface_mesh->vertex_buffer_size, surface_positions, GL_STATIC_DRAW); // tell opengl what data to fill into buffer and how that buffer will be accessed
    //glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); // tell opengl what data to fill into buffer and how that buffer will be accessed


    glEnableVertexAttribArray(0); // enable position attributes of our vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // tell opengl how to read this attribute

    // index buffer
    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer); // because this is an index buffer we must bind it differently
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, surface_mesh->index_buffer_size, surface_indices, GL_STATIC_DRAW); // same when adding data
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // same when adding data

    glBindVertexArray(0); // unbind vao


    // Same for athmospheric mesh ---------------------------------------

    unsigned int athmospheric_vao;
    glGenVertexArrays(1, &athmospheric_vao);
    glBindVertexArray(athmospheric_vao);

    // vertex buffer
    unsigned int athmospheric_vertexBuffer; // buffer id
    glGenBuffers(1, &athmospheric_vertexBuffer); // generate the actual buffer in memory
    glBindBuffer(GL_ARRAY_BUFFER, athmospheric_vertexBuffer); // tell opengl how to handle stride and how to navigate data
    glBufferData(GL_ARRAY_BUFFER, athmospheric_mesh->vertex_buffer_size, athmospheric_positions, GL_STATIC_DRAW); // tell opengl what data to fill into buffer and how that buffer will be accessed

    glEnableVertexAttribArray(0); // enable position attributes of our vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // tell opengl how to read this attribute

    // index buffer
    unsigned int athmospheric_indexBuffer;
    glGenBuffers(1, &athmospheric_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, athmospheric_indexBuffer); // because this is an index buffer we must bind it differently

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, athmospheric_mesh->index_buffer_size, athmospheric_indices, GL_STATIC_DRAW); // same when adding data

    glBindVertexArray(0); // unbind vao


    // Create Program

    ProgramBuilder pb = ProgramBuilder();

    pb.AddShader("res/shaders/vertex-shader.vert", GL_VERTEX_SHADER);
    pb.AddShader("res/shaders/fragment-shader.frag", GL_FRAGMENT_SHADER);
    pb.AddShader("res/shaders/compute-shader.comp", GL_COMPUTE_SHADER);

    // attach shaders
    GLuint program = pb.CompileProgram();

    glUseProgram(program);

    // Uniforms ----------------------------------------------

    // Universal Matrix Uniform

    TransformationModule TM = TransformationModule();

    TM.SetRotation(glm::vec3(0.0, 0.0, 0.0));
    TM.SetScale(glm::vec3(0.4, 0.4, 0.4));
    TM.SetCoordinates(glm::vec3(0.0, 0.0, 0.0));

    glm::mat4x4 transformation_matrix = TM.GetFinalTransformMat();

    int transformation_m_uniform = glGetUniformLocation(program, "translation_matrix");

    glUniformMatrix4fv(transformation_m_uniform, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    float dt = 0.005;
    float t = 0;

    // Color Uniform

    int color_uniform = glGetUniformLocation(program, "u_color");
    glUniform4f(color_uniform, 0.0, 0.0, 0.0, 1.0);

    int object_uniform = glGetUniformLocation(program, "u_object");
    glUniform1i(object_uniform, 0);

    int dimension_uniform = glGetUniformLocation(program, "u_dimension");
    glUniform3i(dimension_uniform, lon, lat, layers);


    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Transformations

        t += ((t + dt) > (2 * PI)) ? dt - (2 * PI) : dt;

        //TM.Rotate(glm::vec3(0.01, 0.01, 0.01));
        //TM.Scale(glm::vec3(0, 0, 0));
        //TM.SetCoordinates(glm::vec3(sinf(t)/2, cos(t)/2, 0.0));

        TM.SetRotation(glm::vec3(sinf(t) * glm::radians(45.0), t, 0));

        glm::mat4x4 transformation_matrix = TM.GetFinalTransformMat();

        glUniformMatrix4fv(transformation_m_uniform, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        glUniform1i(object_uniform, 0);
        glUniform4f(color_uniform, 0.0, 0.0, 0.0, 0.0);
        glBindVertexArray(surface_vao);
        glDrawElements(GL_TRIANGLES, surface_mesh->index_buffer_count * N_VERTEX_P_PRIMITIVE, GL_UNSIGNED_INT, nullptr);

        glUniform1i(object_uniform, 1);
        glUniform4f(color_uniform, 0.0, 1.0, 1.0, 0.1);
        glBindVertexArray(athmospheric_vao);
        glDrawElements(GL_TRIANGLES, athmospheric_mesh->index_buffer_count* N_VERTEX_P_PRIMITIVE, GL_UNSIGNED_INT, nullptr);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}


// Error callback functions
static void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    // ignore list
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return;
    }
    printf("OpenGL Error!\nSource: %x, type: %x, id: %d, severity: %x\nMessage: %s\n\n", source, type, id, severity, message);
}
static void GLFWErrorCallback(int code, const char* description)
{
    printf("GLFW Error!\nCode: %x: %s\n\n", code, description);
}