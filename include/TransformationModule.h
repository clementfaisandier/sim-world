#pragma once

#include <cstdio>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <math.h>

#define X_AXIS glm::vec3(1.0, 0.0, 0.0)
#define Y_AXIS glm::vec3(0.0, 1.0, 0.0)
#define Z_AXIS glm::vec3(0.0, 0.0, 1.0)

/* Transformation Module

This object standarizes the computation of the final transformation matrix.
	The transformation matrix is applied by the vertext shader to ensure all
	objects are rendered correctly relative to the subject camera.

This module accounts for:
	- Subject location.
	- Subject orientation (rotation).
	- Subject scale_factor.
	- Subject perspective (camera fovs).
	- And window shape if defined.
	
This module is slowly starting to become a camera-like module.
*/
class TransformationModule {
public:
	static TransformationModule& getInstance() {
		static TransformationModule instance;
		return instance;
	}
	TransformationModule(const TransformationModule&) = delete;
    TransformationModule& operator=(const TransformationModule&) = delete;
    TransformationModule(TransformationModule&&) = delete;
    TransformationModule& operator=(TransformationModule&&) = delete;

	static void glfwSetKeyCallbackTarget(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// Forward key callbacks to the singleton instance so callbacks affect
		// the global TransformationModule state (instead of a separate static
		// instance local to this function).
		TransformationModule& instance = TransformationModule::getInstance();
		printf("test\n");
		instance.applyUserInput(window, key, scancode, action, mods);
	}

	glm::vec3 setCoordinates(glm::vec3 coordinate);
	glm::vec3 move(glm::vec3 translation);

	glm::vec3 setRotation(glm::vec3 rotation);
	glm::vec3 rotate(glm::vec3 rotation);

	glm::vec3 setScale(glm::vec3 scale_factor);
	glm::vec3 scale(glm::vec3 scale_factor);

	void setWindow(GLFWwindow* window);

	void applyUserInput(GLFWwindow* window, int key, int scancode, int action, int mods);

	void printMatrix(glm::mat4 matrix);

	glm::mat4 getFinalTransformMatrix();

private:
	TransformationModule();

	glm::vec3 coordinate = glm::vec3(0,0,0);
	glm::vec3 rotation = glm::vec3(0,0,0);
	glm::vec3 scale_factor = glm::vec3(1,1,1);
	glm::vec4 perspective = glm::vec4(1.5, 1.0, 0.01, 150.0);
	GLFWwindow* window = nullptr;
};