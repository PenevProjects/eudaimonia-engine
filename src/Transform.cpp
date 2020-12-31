#include "Transform.h"




void chrono::Transform::OnInitialize()
{
	this->position = glm::vec3(0.0);
	this->orientation = glm::vec3(0.0);
	this->scale = glm::vec3(1.0);
}

glm::mat4 chrono::Transform::GetModelMatrix() const
{
	glm::mat4 model(1.0f);

	model = glm::translate(model, position);
	model = glm::rotate(model, orientation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, orientation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, orientation.z, glm::vec3(0, 0, 1));
	model = glm::scale(model, scale);

	return model;
}

void chrono::Transform::setPosition(glm::vec3 _newPosition)
{
	this->position = _newPosition;
}

void chrono::Transform::setOrientation(glm::vec3 _newRotation)
{
	this->orientation = _newRotation;
}

void chrono::Transform::setScale(glm::vec3 _newScale)
{
	this->scale = _newScale;
}

void chrono::Transform::Translate(glm::vec3 _Position)
{
}
