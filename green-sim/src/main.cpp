#include "main.h"


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




// returns the file as a string
// NOTE: RETURNED STRING MUST BE DE-ALLOCATED
static char* ParseShader(const char* filepath)
{
    FILE* file;
    int error_no = fopen_s(&file, filepath, "rb");

    if (error_no != 0 || file == 0) {
        printf("Error whilst reading file: ERRNO #%d", error_no);
        exit(error_no);
    }

    fseek(file, 0, SEEK_END); // I believe this sets the file pointer to the end of the file
    long size = ftell(file); // and this figures out the size of the file using that new poiunter given the file data

    char* fileAsString = (char*)malloc(size + 1);

    fseek(file, 0, SEEK_SET); // reset file stream pointer
    fread(fileAsString, 1, size, file); // places size number of char-sized memory from file into where: places file into where

    fileAsString[size] = 0;

    fclose(file);

    return fileAsString;
}

static unsigned int CompileShader(unsigned int type, char* sourceCode)
{
    unsigned int shader = glCreateShader(type); // create shader object of given type
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        std::cout << "Shader did not compile successfuly: " << (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex");
        exit(1);
    }

    return shader;
}

// this method is going to take some shader code in string format and make the shader, then link them into a single program
static unsigned int CreateProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int program = glCreateProgram(); // create program object to which shaders can be attached

    glAttachShader(program, vertexShader); // attach
    glAttachShader(program, fragmentShader);

    // linking process
    glLinkProgram(program);

    // verify linking process
    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        int max_length = 200;
        char* log = (char*)malloc(max_length);
        int length;

        glGetProgramInfoLog(program, max_length, &length, log);

        log[length] = 0;
        printf("Linking Error: %s\n\n", log);

        free(log);
    }

    glValidateProgram(program);

    // verify validation process

    glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int max_length = 200;
        char* log = (char*)malloc(max_length);
        int length;

        glGetProgramInfoLog(program, max_length, &length, log);

        log[length] = 0;
        printf("Validation Error: %s\n\n", log);

        free(log);
    }

    return program;
}


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

   //glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    SphericalMeshBuilder mesh_builder = SphericalMeshBuilder(1.0, 1.3, 3, 2, 3);

    mesh_builder.Print();

    Mesh* mesh = mesh_builder.GetSurfaceMesh();

    mesh->Print();

    float man_pos[15] = { 0.0, 0.0, 0.5,
                            0.0, 1.0, 0.0,
                            0.866025, -0.500000, -0.000000,
                            -0.866025, -0.500000, -0.000000,
                            0.0, 0.0, -1.0, };

    unsigned int man_ind[18] = { 0, 2, 1,
                                0, 3, 2,
                                0, 1, 3,
                                4, 1, 2,
                                4, 2, 3,
                                4, 3, 1 };

    float* positions = mesh->vertex_buffer;
    unsigned int* indices = mesh->index_buffer;

    //float* positions = man_pos;
    //unsigned int* indices = man_ind;

    unsigned int index_count = mesh->index_buffer_count;

    

    // create VAO
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertex buffer
    unsigned int vertexBuffer; // buffer id
    glGenBuffers(1, &vertexBuffer); // generate the actual buffer in memory
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // tell opengl how to handle stride and how to navigate data
    glBufferData(GL_ARRAY_BUFFER, mesh->vertex_buffer_size, positions, GL_STATIC_DRAW); // tell opengl what data to fill into buffer and how that buffer will be accessed
    //glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); // tell opengl what data to fill into buffer and how that buffer will be accessed


    glEnableVertexAttribArray(0); // enable position attributes of our vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // tell opengl how to read this attribute

    // index buffer
    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer); // because this is an index buffer we must bind it differently
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer_size, indices, GL_STATIC_DRAW); // same when adding data
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // same when adding data



    // creating shaders

    // get source code
    char* vertexShaderSource = ParseShader("res/shaders/vertex-shader.vert");
    char* fragmentShaderSource = ParseShader("res/shaders/fragment-shader.frag");

    // compile shaders
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    free(vertexShaderSource);
    free(fragmentShaderSource);
    //earth_tensor.~SphericalTensor();

    // attach shaders
    unsigned int program = CreateProgram(vertexShader, fragmentShader);

    glUseProgram(program);
    // Uniforms

    TransformationModule TM = TransformationModule();

    TM.SetScale(glm::vec3(0.3, 0.3, 0.3));



    glm::mat4x4 transformation_matrix = TM.GetFinalTransformMat();

    int transformation_m_uniform = glGetUniformLocation(program, "translation_matrix");

    glUniformMatrix4fv(transformation_m_uniform, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    float dt = 0.005;
    float t = 0;

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, mesh->index_buffer_count * mesh->primitive_num_components, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_TRIANGLES, sizeof(indices)/4, GL_UNSIGNED_INT, nullptr);


        t += ((t + dt) > (2 * PI)) ? dt - (2 * PI) : dt;

        //printf("%f\n", t);

        TM.Rotate(glm::vec3(0.01, 0.01, 0.01));
        TM.Scale(glm::vec3(0, 0, 0));
        TM.SetCoordinates(glm::vec3(sinf(t)/2, cos(t)/2, 0));

        glm::mat4x4 transformation_matrix = TM.GetFinalTransformMat();

        glUniformMatrix4fv(transformation_m_uniform, 1, GL_FALSE, glm::value_ptr(transformation_matrix));


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}