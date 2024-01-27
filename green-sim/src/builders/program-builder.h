#pragma once

#include <iostream>
#include <queue>

#include <GL\glew.h>


class ProgramBuilder {


public:
	ProgramBuilder();
	void AddShader(constchar* file_path, GLenum shader_type);
	GLuint CompileProgram();
	

private:
	GLuint program;
	std::queue<GLuint> shaders{};

	char* ParseShader(const char* file_path);
	GLuint CompileShader(const char* source_code, GLenum shader_type);

};