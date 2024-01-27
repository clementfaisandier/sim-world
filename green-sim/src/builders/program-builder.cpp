#include "program-builder.h"


ProgramBuilder::ProgramBuilder() {
    program = glCreateProgram(); // create program object to which shaders can be attached

    if (program == 0)
    {
        printf("Error creating program object. GL ErrorNo: %d\n\n", glGetError());
        exit(1);
    }
}
void ProgramBuilder::AddShader(const char* file_path, GLenum shader_type) {

    char* source_code = ParseShader(file_path);

    unsigned int shader = CompileShader(source_code, shader_type);

    shaders.push(shader);

    free(source_code);

}
GLuint ProgramBuilder::CompileProgram() {

    while (!shaders.empty()) {
        glAttachShader(program, shaders.front());
        shaders.pop();
    }

    // linking process
    glLinkProgram(program);

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

    // validation process
    glValidateProgram(program);

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

char* ProgramBuilder::ParseShader(const char* file_path) {

    FILE* file;
    int error_no = fopen_s(&file, file_path, "rb");

    if (error_no != 0 || file == 0) {
        printf("Error whilst reading file: ERRNO #%d", error_no);
        exit(error_no);
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

GLuint ProgramBuilder::CompileShader(const char* source_code, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type); // create shader object of given type
    if (shader == 0)
    {
        printf("Error creating shader object. GL ErrorNo: %d\n\n", glGetError());
        exit(1);
    }
    glShaderSource(shader, 1, &source_code, NULL);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        std::string s_type;

        switch (shader_type) {
        case GL_VERTEX_SHADER:
            s_type = "Vector Shader";
            break;
        case GL_FRAGMENT_SHADER:
            s_type = "Fragment Shader";
            break;
        case GL_COMPUTE_SHADER:
            s_type = "Compute Shader";
            break;
        default:
            s_type = "Unsupported Shader Type?";
        }
        std::cout << "Shader did not compile successfuly: " << s_type << std::endl;
        exit(1);
    }

    return shader;
}