#include "main.h"

int main(void)
{

    Settings sts{};

    GLFWwindow* window = GLFWInitialization(sts);

    GLInitialization(sts);

    std::cout << glGetString(GL_VERSION) << std::endl << glGetString(GL_RENDERER) << std::endl;

    // Graphics Program ---------------------------------------
    ProgramBuilder pb = ProgramBuilder();

    pb.AddShader("res/shaders/vertex-shader.vert", GL_VERTEX_SHADER);
    pb.AddShader("res/shaders/fragment-shader.frag", GL_FRAGMENT_SHADER);

    GLuint graphics_program = pb.CompileProgram();
    glUseProgram(graphics_program);

    // Compute Program ---------------------------------------
    {
        GLint gl_max_compute_group_count_x = -1;
        GLint gl_max_compute_group_count_y = -1;
        GLint gl_max_compute_group_count_z = -1;

        GLint gl_max_compute_group_size_x = -1;
        GLint gl_max_compute_group_size_y = -1;
        GLint gl_max_compute_group_size_z = -1;

        GLint gl_max_compute_work_group_invocations = -1;

        GLint gl_max_compute_shared_memory_size = -1;

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &gl_max_compute_group_count_x);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &gl_max_compute_group_count_y);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &gl_max_compute_group_count_z);

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &gl_max_compute_group_size_x);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &gl_max_compute_group_size_y);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &gl_max_compute_group_size_z);

        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &gl_max_compute_work_group_invocations);

        glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &gl_max_compute_shared_memory_size);

        std::cout << "GL_MAX_COMPUTE_WORK_GROUP_COUNT: " << gl_max_compute_group_count_x << " "
            << gl_max_compute_group_count_y << " " << gl_max_compute_group_count_z << std::endl << std::endl;

        std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE: " << gl_max_compute_group_size_x << " "
            << gl_max_compute_group_size_y << " " << gl_max_compute_group_size_z << std::endl << std::endl;

        std::cout << "GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS: " << gl_max_compute_work_group_invocations << std::endl << std::endl;

        std::cout << "GL_MAX_COMPUTE_SHARED_MEMORY_SIZE: " << gl_max_compute_shared_memory_size << std::endl << std::endl;
    }

    pb = ProgramBuilder();

    pb.AddShader("res/shaders/compute-shader.comp", GL_COMPUTE_SHADER);

    GLuint compute_program = pb.CompileProgram();



    // MESHES AND VAOS ---------------------------------------
    
    SphericalMeshBuilder mesh_builder = SphericalMeshBuilder(sts.lon, sts.lat, sts.layers, sts.scale_min, sts.scale_max);

    SphericalGraphicsMesh* surface_mesh = mesh_builder.GetSurfaceMesh();
    SphericalGraphicsMesh* athmospheric_mesh = mesh_builder.GetAthmosphericMesh();
    athmospheric_mesh->PrintMeta();

    SphericalComputeMesh* compute_mesh = mesh_builder.GetComputeMesh();
    compute_mesh->PrintMeta();

    glm::vec3* surface_positions = surface_mesh->vertex_buffer;
    glm::vec4* surface_colors = surface_mesh->color_buffer;
    unsigned int* surface_indices = surface_mesh->index_buffer;

    glm::vec3* athmospheric_positions = athmospheric_mesh->vertex_buffer;
    glm::vec4* athmospheric_colors = athmospheric_mesh->color_buffer;
    unsigned int* athmospheric_indices = athmospheric_mesh->index_buffer;

    SphericalComputeMesh::Cell* compute_buffer = compute_mesh->compute_buffer;




    // Surface Mesh VAO ---------------------------------------
    // 
    // NOTE: VAO contain VERTEX_ATTRIB_ARRY_BUFFER_BINDING points (and more ofc), which reference the actual buffer objects.
    // 
    // Because my buffers are not interweaved => each vertex attribute will have a continuous buffer bound to it.
    // 
    // Meaning: Vertex attribute state should reflect the packed buffer => stride = 0, offset = 0.
    // 
    // 
    // create VAO
    GLuint surface_vao;
    glGenVertexArrays(1, &surface_vao);
    glBindVertexArray(surface_vao);

    // vertex buffer
    GLuint vertex_buffer; // buffer id
    glGenBuffers(1, &vertex_buffer); // generate the actual buffer in memory
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); // tell opengl how to handle stride and how to navigate data
    glBufferData(GL_ARRAY_BUFFER, surface_mesh->vertex_buffer_size, surface_positions, GL_STATIC_DRAW); // tell opengl what data to fill into buffer and how that buffer will be accessed
    glEnableVertexAttribArray(0); // enable position attributes of our vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // tell opengl how to read this attribute
    
    // color buffer
    GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, surface_mesh->color_buffer_size, surface_colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    // index buffer
    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); // because this is an index buffer we must bind it differently
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, surface_mesh->index_buffer_size, surface_indices, GL_STATIC_DRAW); // same when adding data


    // Athmospheric Meshes ---------------------------------------
    
    
     
    // Athmospheric Mesh VAO ---------------------------------------

    GLuint athmospheric_vao;
    glGenVertexArrays(1, &athmospheric_vao);
    glBindVertexArray(athmospheric_vao);

    // vertex buffer
    GLuint athmospheric_vertex_buffer; // buffer id
    glGenBuffers(1, &athmospheric_vertex_buffer); // generate the actual buffer in memory
    glBindBuffer(GL_ARRAY_BUFFER, athmospheric_vertex_buffer); // tell opengl how to handle stride and how to navigate data
    glBufferData(GL_ARRAY_BUFFER, athmospheric_mesh->vertex_buffer_size, athmospheric_positions, GL_STATIC_DRAW); // tell opengl what data to fill into buffer and how that buffer will be accessed
    glEnableVertexAttribArray(0); // enable position attributes of our vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // tell opengl how to read this attribute

    // color buffer
    GLuint athmospheric_color_buffer;
    glGenBuffers(1, &athmospheric_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, athmospheric_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, athmospheric_mesh->color_buffer_size, athmospheric_colors, GL_DYNAMIC_COPY);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // index buffer
    GLuint athmospheric_index_buffer;
    glGenBuffers(1, &athmospheric_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, athmospheric_index_buffer); // because this is an index buffer we must bind it differently

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, athmospheric_mesh->index_buffer_size, athmospheric_indices, GL_STATIC_DRAW); // same when adding data




    // Athmospheric Compute Mesh ---------------------------------------
    
    GLuint compute_buffers[2];
    glGenBuffers(2, compute_buffers);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compute_buffers[0]);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, compute_buffers[1]);

    glNamedBufferData(compute_buffers[0], compute_mesh->compute_buffer_size, compute_mesh->compute_buffer, GL_DYNAMIC_COPY);
    glNamedBufferData(compute_buffers[1], compute_mesh->compute_buffer_size, compute_mesh->compute_buffer, GL_DYNAMIC_COPY);


    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, compute_buffers[0]); // bind the buffer GL_SHADER_STORAGE_BUFFER binding point 0
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, compute_buffers[1]); // bind the buffer GL_SHADER_STORAGE_BUFFER binding point 0
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, athmospheric_color_buffer); // bind the buffer GL_SHADER_STORAGE_BUFFER binding point 2
    

    // Retrieving buffer metadata
    GLint buffer_size;
    glGetNamedBufferParameteriv(compute_buffers[0], GL_BUFFER_SIZE, &buffer_size);
    printf("compute_buffers[0]: %d\n", buffer_size/sizeof(*compute_mesh->compute_buffer));
    glGetNamedBufferParameteriv(compute_buffers[1], GL_BUFFER_SIZE, &buffer_size);
    printf("compute_buffers[1]: %d\n", buffer_size);
    glGetNamedBufferParameteriv(athmospheric_color_buffer, GL_BUFFER_SIZE, &buffer_size);
    printf("athmospheric_color_buffer: %d\n", buffer_size);



    // Uniforms For Graphics Program ----------------------------------------------

    // Universal Matrix Uniform

    TransformationModule TM = TransformationModule();

    TM.SetRotation(glm::vec3(0.0, 0.0, 0.0));
    TM.SetScale(glm::vec3(0.4, 0.4, 0.4));
    TM.SetCoordinates(glm::vec3(0.0, 0.0, 0.0));

    glm::mat4x4 transformation_matrix = TM.GetFinalTransformMat();

    GLuint transformation_m_uniform = glGetUniformLocation(graphics_program, "translation_matrix");

    glUniformMatrix4fv(transformation_m_uniform, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    float dt = 0.005;
    float t = 0;


    // Uniforms for Compute Program -----------------

    bool double_buffer_toggle = 0;
    GLuint double_buffer_toggle_uniform = glGetUniformLocation(compute_program, "double_buffer_toggle");
    glUniform1d(double_buffer_toggle_uniform, double_buffer_toggle);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {

        // Compute Pipeline
        glUseProgram(compute_program);

        glDispatchCompute(compute_mesh->compute_buffer_count, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

        // Swap Buffers
        double_buffer_toggle = !double_buffer_toggle;

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, compute_buffers[double_buffer_toggle]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, compute_buffers[!double_buffer_toggle]);




        // Graphics Pipeline
        glUseProgram(graphics_program);

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


        glBindVertexArray(surface_vao);
        glDrawElements(GL_TRIANGLES, surface_mesh->index_buffer_count * N_VERTEX_P_PRIMITIVE, GL_UNSIGNED_INT, nullptr);

        
        glBindVertexArray(athmospheric_vao);
        glDrawElements(GL_TRIANGLES, athmospheric_mesh->index_buffer_count* N_VERTEX_P_PRIMITIVE, GL_UNSIGNED_INT, nullptr);

        
        
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(graphics_program);
    //glDeleteProgram(compute_program);

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

static GLFWwindow* GLFWInitialization(Settings sts) {

    // Initialize the library
    if (!glfwInit()) {
        std::cout << "Unable to initialize GLFW" << std::endl;
        exit(-1);
    }

    glfwSetErrorCallback(GLFWErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, sts.GL_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, sts.GL_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(sts.window_width, sts.window_height, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Unable to start window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    // Make the window's context the current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    return window;

}

static void GLInitialization(Settings sts) {

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
}