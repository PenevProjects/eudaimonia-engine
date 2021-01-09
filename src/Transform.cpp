#include "Transform.h"

void Transform::setup(glm::vec3 _position, glm::vec3 _orientation, glm::vec3 _scale)
{
	this->position = _position;
	this->orientation = _orientation;
	this->scale = _scale;
}