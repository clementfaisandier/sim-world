#include "transformation-module.h"
#include <glm/gtc/type_ptr.hpp>

TransformationModule::TransformationModule() {
	coordinate = glm::vec3(0,0,0);
	rotation = glm::vec3(0,0,0);
	scale = glm::vec3(1,1,1);
	perspective = glm::vec4(1.5, 1.0, 0.01, 150.0);
}

TransformationModule::TransformationModule(glm::vec3 coordinate, glm::vec3 rotation, glm::vec3 scale) {
	this->coordinate = coordinate;
	this->rotation = rotation;
	this->scale = scale;
}

glm::vec3 TransformationModule::SetCoordinates(glm::vec3 coordinate) {
	this->coordinate = coordinate;
	return this->coordinate;
}
glm::vec3 TransformationModule::Move(glm::vec3 translation) {
	this->coordinate += translation;
	return this->coordinate;
}

glm::vec3 TransformationModule::SetRotation(glm::vec3 rotation) {
	this->rotation = rotation;
	return this->rotation;
}
glm::vec3 TransformationModule::Rotate(glm::vec3 rotation) {
	this->rotation += rotation;

	return this->rotation;
}

glm::vec3 TransformationModule::SetScale(glm::vec3 scale) {
	this->scale = scale;
	return this->scale;
}
glm::vec3 TransformationModule::Scale(glm::vec3 scale) {
	this->scale += scale;
	return this->scale;
}

glm::vec4 TransformationModule::SetPerspective(float aspect_ratio, float vertical_fov, float near_z_plane, float far_z_plane) {
	perspective = glm::vec4(aspect_ratio, vertical_fov, near_z_plane, far_z_plane);
	return perspective;
}

void TransformationModule::PrintMatrix(glm::mat4 matrix) {
	int index = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++, index++) {
			printf("%f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


glm::mat4 TransformationModule::GetFinalTransformMat() {

	glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0), scale);

	glm::mat4 x_rotation_matrix = glm::rotate(glm::mat4(1.0), rotation.x, X_AXIS);

	glm::mat4 y_rotation_matrix = glm::rotate(glm::mat4(1.0), rotation.y, Y_AXIS);
	
	glm::mat4 z_rotation_matrix = glm::rotate(glm::mat4(1.0), rotation.z, Z_AXIS);

	glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0), coordinate);

	//glm::mat4 perspective_matrix = glm::perspective(perspective[0], perspective[1], perspective[2], perspective[3]);

	glm::mat4 final_matrix = (translation_matrix * x_rotation_matrix * y_rotation_matrix * z_rotation_matrix * scaling_matrix);
	//final_matrix = perspective_matrix * final_matrix;

	if (false) {
		printf("matrices: \n");
		PrintMatrix(scaling_matrix);
		PrintMatrix(x_rotation_matrix);
		PrintMatrix(y_rotation_matrix);
		PrintMatrix(z_rotation_matrix);
		PrintMatrix(translation_matrix);
		PrintMatrix(final_matrix);
	}


	return final_matrix;
	// could add a post-translation matrix for orbits one day-> could be fun but definitely uncecessary atm
}



