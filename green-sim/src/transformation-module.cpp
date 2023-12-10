#include "transformation-module.h"
#include <glm/gtc/type_ptr.hpp>

TransformationModule::TransformationModule() {
	coordinate = glm::vec3();
	coordinate = glm::vec3();
	scale = glm::vec3();
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

glm::mat4 TransformationModule::GetFinalTransformMat() {

	glm::mat4x4 scaling_matrix = glm::translate(glm::mat4(1.0), scale);

	glm::mat4x4 rotation_matrix = glm::rotate(scaling_matrix, rotation.x, X_AXIS);
	rotation_matrix = glm::rotate(rotation_matrix, rotation.y, Y_AXIS);
	rotation_matrix = glm::rotate(rotation_matrix, rotation.z, Z_AXIS);

	glm::mat4x4 translation_matrix = glm::translate(rotation_matrix, coordinate);

	return translation_matrix;
	// could add a post-translation matrix for orbits one day-> could be fun but definitely uncecessary atm
}


