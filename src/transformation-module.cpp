#include "transformation-module.h"
#include <glm/gtc/type_ptr.hpp>

TransformationModule::TransformationModule() {}

glm::vec3 TransformationModule::setCoordinates(glm::vec3 coordinate) {
	this->coordinate = coordinate;
	return this->coordinate;
}
glm::vec3 TransformationModule::move(glm::vec3 translation) {
	this->coordinate += translation;
	return this->coordinate;
}

glm::vec3 TransformationModule::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
	return this->rotation;
}
glm::vec3 TransformationModule::rotate(glm::vec3 rotation) {
	this->rotation += rotation;

	return this->rotation;
}

glm::vec3 TransformationModule::setScale(glm::vec3 scale_factor) {
	this->scale_factor = scale_factor;
	return this->scale_factor;
}
glm::vec3 TransformationModule::scale(glm::vec3 scale_factor) {
	this->scale_factor += scale_factor;
	return this->scale_factor;
}

void TransformationModule::setWindow(GLFWwindow* window) {
	this->window = window;
	glfwSetKeyCallback(window, TransformationModule::glfwSetKeyCallbackTarget);
}

void TransformationModule::applyUserInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	float rotation_speed = 0.1;
	float zoom_speed = 0.1;
	if (key == GLFW_KEY_UP) {
		rotate(glm::vec3(0.0, 0.0, rotation_speed));
	}
	else if (key == GLFW_KEY_DOWN) {
		rotate(glm::vec3(0.0, 0.0, -rotation_speed));
	}
	else if (key == GLFW_KEY_LEFT) {
		rotate(glm::vec3(0.0, rotation_speed, 0.0));
	}
	else if (key == GLFW_KEY_RIGHT) {
		rotate(glm::vec3(0.0, -rotation_speed, 0.0));
	}
	else if (key == GLFW_KEY_PAGE_UP) {
		scale(glm::vec3(zoom_speed));
	}
	else if (key == GLFW_KEY_PAGE_DOWN) {
		scale(glm::vec3(-zoom_speed));
	}
}


void TransformationModule::printMatrix(glm::mat4 matrix) {
	int index = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++, index++) {
			printf("%f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


glm::mat4 TransformationModule::getFinalTransformMatrix() {

	glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0), scale_factor);

	glm::mat4 x_rotation_matrix = glm::rotate(glm::mat4(1.0), rotation.x, X_AXIS);

	glm::mat4 y_rotation_matrix = glm::rotate(glm::mat4(1.0), rotation.y, Y_AXIS);
	
	glm::mat4 z_rotation_matrix = glm::rotate(glm::mat4(1.0), rotation.z, Z_AXIS);

	glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0), coordinate);

	glm::mat4 window_normalization_matrix = glm::mat4(1.0);
	// if (window != nullptr) {
	// 	int height = 0;
	// 	int width = 0;
	// 	glfwGetWindowSize(window, &width, &height);
	// 	float aspect_ratio = width/height;
	// 	window_normalization_matrix = glm::perspective(
	// 		glm::radians(45.0f),
	// 		aspect_ratio,
	// 		0.01f,
	// 		10.0f
	// 	);
	// }

	glm::mat4 final_matrix = (translation_matrix * x_rotation_matrix * y_rotation_matrix * z_rotation_matrix * scaling_matrix * window_normalization_matrix);

	return final_matrix;
}



