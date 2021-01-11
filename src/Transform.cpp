#include "Transform.h"

void Transform::setup(glm::vec3 _position, glm::vec3 _euler_angles, glm::vec3 _scale)
{
	this->position = _position;
	this->euler_angles = _euler_angles;
	this->scale = _scale;
	this->rotation_internal_ = glm::quat(_euler_angles);
	this->forward_ = glm::vec3(0.0f, 0.0f, -1.0f);
	this->right_ = glm::vec3(1.0f, 0.0f, 0.0f);
	this->up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	this->model_matrix_ = glm::mat4();

}