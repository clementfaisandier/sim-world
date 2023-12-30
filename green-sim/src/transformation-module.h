#pragma once


#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define X_AXIS glm::vec3(1.0, 0.0, 0.0)
#define Y_AXIS glm::vec3(0.0, 1.0, 0.0)
#define Z_AXIS glm::vec3(0.0, 0.0, 1.0)

class TransformationModule {

public:
	glm::vec3 coordinate;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec4 perspective;

	TransformationModule();
	TransformationModule(glm::vec3 coordinate, glm::vec3 rotation, glm::vec3 scale);

	glm::vec3 SetCoordinates(glm::vec3 coordinate);
	glm::vec3 Move(glm::vec3 translation);

	glm::vec3 SetRotation(glm::vec3 rotation);
	glm::vec3 Rotate(glm::vec3 rotation);

	glm::vec3 SetScale(glm::vec3 scale);
	glm::vec3 Scale(glm::vec3 scale);

	glm::vec4 SetPerspective(float aspect_ratio, float vertical_fov, float near_z_plane, float far_z_plane);

	void PrintMatrix(glm::mat4 matrix);

	glm::mat4 GetFinalTransformMat();
};