#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>
#include "spherical-mesh.h"
#include "math.h"

#include <cmath>
#define PI 3.14159265
#define D_TO_RAD 0.01745329251


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
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);

    //glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    SphericalMesh earth_tensor = SphericalMesh(3 ,2);

    float* positions = earth_tensor.GetVertexBuffer();
    unsigned int* indices = earth_tensor.GetIndexBuffer();

    unsigned int index_count = earth_tensor.GetIndexBufferCount();


    if (DEBUG) earth_tensor.PrintDrawOrder();
    

    // create VAO
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertex buffer
    unsigned int vertexBuffer; // buffer id
    glGenBuffers(1, &vertexBuffer); // generate the actual buffer in memory
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // tell opengl how to handle stride and how to navigate data
    glBufferData(GL_ARRAY_BUFFER, earth_tensor.GetVertexBufferSize(), positions, GL_STATIC_DRAW); // tell opengl what data to fill into buffer and how that buffer will be accessed
    //glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); // tell opengl what data to fill into buffer and how that buffer will be accessed


    glEnableVertexAttribArray(0); // enable position attributes of our vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // tell opengl how to read this attribute

    // index buffer
    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer); // because this is an index buffer we must bind it differently
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, earth_tensor.GetIndexBufferSize(), indices, GL_STATIC_DRAW); // same when adding data
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

    // matrix

    //Matrix4f matrix = Matrix4f(Matrix4f::TRANSLATION_MODE, 0.1, 0.1, 0.1);

    
    // translation matrix
    
    float x_d = 0;
    float y_d = 0;
    float z_d = 0;
    float trans_matrix[16] = {
                        1.0, 0.0, 0.0, 0.1,
                        0.0, 1.0, 0.0, 0.1,
                        0.0, 0.0, 1.0, 0.1,
                        0.0, 0.0, 0.0, 1.0
    };
    
    // rotational matrix

    float x_rot = 0;
    float y_rot = 0;
    float z_rot = 0;
    float x_rot_d = 0;
    float y_rot_d = PI / 180;
    float z_rot_d = 0;

    float z_rot_matrix[16] = {
                        cosf(z_rot), -sinf(z_rot), 0.0, 0.0,
                        sinf(z_rot), cosf(z_rot), 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0
    };
    float y_rot_matrix[16] = {
                        cosf(y_rot), 0.0, -sinf(y_rot), 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        sinf(y_rot), 0.0, cosf(y_rot), 0.0,
                        0.0, 0.0, 0.0, 1.0
    };
    float x_rot_matrix[16] = {
                       1.0, 0.0, 0.0, 0.0,
                       0.0, cosf(x_rot), -sinf(x_rot), 0.0,
                       0.0, sinf(x_rot), cosf(x_rot), 0.0,
                       0.0, 0.0, 0.0, 1.0
    };
    

    int trans_m_uniform = glGetUniformLocation(program, "translation_matrix");
    int x_rot_m_uniform = glGetUniformLocation(program, "x_rot_matrix");
    int y_rot_m_uniform = glGetUniformLocation(program, "y_rot_matrix");
    int z_rot_m_uniform = glGetUniformLocation(program, "z_rot_matrix");

    glUniformMatrix4fv(trans_m_uniform, 1, GL_TRUE, trans_matrix);
    glUniformMatrix4fv(x_rot_m_uniform, 1, GL_TRUE, x_rot_matrix);
    glUniformMatrix4fv(y_rot_m_uniform, 1, GL_TRUE, y_rot_matrix);
    glUniformMatrix4fv(z_rot_m_uniform, 1, GL_TRUE, z_rot_matrix);


    // color Uniform

    float red = 0.0f;
    float red_shift = 0.01f;
    float green = 0.5f;
    float green_shift = 0.001f;
    float blue = 1.0f;
    float blue_shift = 0.01f;

    int uniform = glGetUniformLocation(program, "u_color"); // get uniform id from shader
    if (uniform == -1) { std::cout << "Uniform u_color does not exist"; }

    glUniform4f(uniform, red, green, blue, 1.0f); // link new uniform

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, index_count*3, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_TRIANGLES, sizeof(indices)/4, GL_UNSIGNED_INT, nullptr);

        red += red_shift;
        green += green_shift;   
        blue += blue_shift;

        if (red > 1.0f || red < 0.0f)
        {
            red_shift = -red_shift;
            red += red_shift;
        }
        if (green > 1.0 || green < 0.0f)
        {
            green_shift = -green_shift;
            green += green_shift;
        }
        if (blue > 1.0 || blue < 0.0f)
        {
            blue_shift = -blue_shift;
            blue += blue_shift;
        }

        trans_matrix[3] += x_d;
        trans_matrix[7] += y_d;
        trans_matrix[11] += z_d;

        z_rot += z_rot_d;
        y_rot += y_rot_d;
        x_rot += x_rot_d;

        z_rot_matrix[0] = cosf(z_rot);
        z_rot_matrix[1] = -sinf(z_rot);
        z_rot_matrix[4] = sinf(z_rot);
        z_rot_matrix[5] = cosf(z_rot);

        y_rot_matrix[0] = cosf(y_rot);
        y_rot_matrix[2] = -sinf(y_rot);
        y_rot_matrix[8] = sinf(y_rot);
        y_rot_matrix[10] = cosf(y_rot);

        x_rot_matrix[5] = cosf(x_rot);
        x_rot_matrix[6] = -sinf(x_rot);
        x_rot_matrix[9] = sinf(x_rot);
        x_rot_matrix[10] = cosf(x_rot);

        glUniform4f(uniform, red, green, blue, 1.0f);
        glUniformMatrix4fv(trans_m_uniform, 1, GL_TRUE, trans_matrix);
        glUniformMatrix4fv(x_rot_m_uniform, 1, GL_TRUE, x_rot_matrix);
        glUniformMatrix4fv(y_rot_m_uniform, 1, GL_TRUE, y_rot_matrix);
        glUniformMatrix4fv(z_rot_m_uniform, 1, GL_TRUE, z_rot_matrix);


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}