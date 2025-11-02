#include "main.h"

#define PI 3.14159265358979323846


int main(void)
{
    /////////////////////
    // Initialize GLFW //
    /////////////////////
    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(glfwErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
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
    glDebugMessageCallback(glErrorCallback, 0);

    // face culling optimization -> can lead to invisible triangles if the index buffer defines the trianges in a counter-clockwise fashion
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    // blending for alpha channel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);

    std::cout << glGetString(GL_VERSION) << std::endl;

    ///////////////////
    // Define Meshes //
    ///////////////////

    // MESHES AND VAOS ---------------------------------------

    int lon = 90;
    int lat = 45;
    int layers = 10;
    int scale_min = 1.0;
    int scale_max = 2.0;
    
    SphericalMeshBuilder mesh_builder = SphericalMeshBuilder(lon, lat, layers, scale_min, scale_max);

    SphericalGraphicsMesh* surface_mesh = mesh_builder.getSurfaceMesh();
    SphericalGraphicsMesh* athmospheric_mesh = mesh_builder.getAthmosphericMesh();

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

    
    ////////////////////
    // Define Shaders //
    ////////////////////
    /*
    char* computeShaderSource = ParseShader("shaders/compute-shader.comp");

    int computeShader = CompileShader(GL_COMPUTE_SHADER, computeShaderSource);

    free(computeShaderSource);

    unsigned int computeProgram = CreateComputeProgram(computeShader);*/

    // get source code
    char* vertexShaderSource = parseShader("shaders/vertex-shader.vert");
    char* fragmentShaderSource = parseShader("shaders/fragment-shader.frag");

    // compile shaders
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    free(vertexShaderSource);
    free(fragmentShaderSource);
    //earth_tensor.~SphericalTensor();

    // attach shaders
    unsigned int program = createProgram(vertexShader, fragmentShader);

    glUseProgram(program);

    //////////////
    // Uniforms //
    //////////////

    // Universal Matrix Uniform

    TransformationModule* TM = &TransformationModule::getInstance();

    // Register the GLFW window with the TransformationModule so it can
    // receive key callbacks via glfwSetKeyCallback inside setWindow().
    TM->setWindow(window);

    TM->setRotation(glm::vec3(0.0, 0.0, 0.0));
    TM->setScale(glm::vec3(0.4, 0.4, 0.4));
    TM->setCoordinates(glm::vec3(0.0, 0.0, 0.0));

    glm::mat4x4 transformation_matrix = TM->getFinalTransformMatrix();

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

    //////////////////////
    // Application Loop //
    //////////////////////

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Transformations
        // t += ((t + dt) > (2 * PI)) ? dt - (2 * PI) : dt;
        //TM->rotate(glm::vec3(0.01, 0.01, 0.01));
        //TM->scale(glm::vec3(0, 0, 0));
        // TM->setCoordinates(glm::vec3(sinf(t)/2, cos(t)/2, 0.0));
        // TM->setRotation(glm::vec3(sinf(t) * glm::radians(45.0), t, 0));
        glm::mat4x4 transformation_matrix = TM->getFinalTransformMatrix();
        glUniformMatrix4fv(transformation_m_uniform, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        // Draw earth surface
        glUniform1i(object_uniform, 0);
        glUniform4f(color_uniform, 0.0, 0.0, 0.0, 0.0);
        glBindVertexArray(surface_vao);
        glDrawElements(GL_TRIANGLES, surface_mesh->index_buffer_count * N_VERTEX_P_PRIMITIVE, GL_UNSIGNED_INT, nullptr);

        // Draw athmospere surface
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
static void glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    // ignore list
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return;
    }
    printf("OpenGL Error!\nSource: %x, type: %x, id: %d, severity: %x\nMessage: %s\n\n", source, type, id, severity, message);
}
static void glfwErrorCallback(int code, const char* description)
{
    printf("GLFW Error!\nCode: %x: %s\n\n", code, description);
}


// returns the file as a string
// NOTE: RETURNED STRING MUST BE DE-ALLOCATED
static char* parseShader(const char* filepath)
{
    FILE* file = std::fopen(filepath, "rb");
    if (!file) {
        printf("Error whilst reading file: ERRNO #%d", errno);
        exit(errno);
    }

    fseek(file, 0, SEEK_END); // I believe this sets the file pointer to the end of the file
    long size = ftell(file); // and this figures out the size of the file using that new poiunter given the file data

    char* fileAsString = (char*)malloc(size + 1);
    if (fileAsString == 0)
    {
		printf("Error allocating memory for file string.");
		exit(1);
	}

    fseek(file, 0, SEEK_SET); // reset file stream pointer
    fread(fileAsString, 1, size, file); // places size number of char-sized memory from file into where: places file into where. warning is missunderstanding

    fileAsString[size] = 0; // null terminate string

    fclose(file);

    return fileAsString;
}

static unsigned int compileShader(unsigned int type, char* sourceCode)
{
    unsigned int shader = glCreateShader(type); // create shader object of given type
    if (shader == 0)
    {
		printf("Error creating shader object. GL ErrorNo: %d\n\n", glGetError());
		exit(1);
	}
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

static unsigned int CreateComputeProgram(unsigned int computeShader) {

    unsigned int program = glCreateProgram(); // create program object to which shaders can be attached

    if (program == 0)
    {
        printf("Error creating program object. GL ErrorNo: %d\n\n", glGetError());
        exit(1);
    }

    glAttachShader(program, computeShader);

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
        if (log == 0) {
            printf("Error allocating memory for log string: program compilation error also occured.");
            exit(1);
        }
        int length;

        glGetProgramInfoLog(program, max_length, &length, log);

        log[length] = 0;
        printf("Validation Error: %s\n\n", log);

        free(log);
    }

    return program;
}

// this method is going to take some shader code in string format and make the shader, then link them into a single program
static unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int program = glCreateProgram(); // create program object to which shaders can be attached

    if (program == 0)
    {
		printf("Error creating program object. GL ErrorNo: %d\n\n", glGetError());
		exit(1);
	}

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
        if (log == 0) {
            printf("Error allocating memory for log string: program compilation error also occured.");
			exit(1);
        }
        int length;

        glGetProgramInfoLog(program, max_length, &length, log);

        log[length] = 0;
        printf("Validation Error: %s\n\n", log);

        free(log);
    }

    return program;
}