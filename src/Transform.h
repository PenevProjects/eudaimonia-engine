#ifndef _TRANSFORM_COMPONENT_H
#define _TRANSFORM_COMPONENT_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <memory>

#include "zero/Component.h"


struct Transform : public zero::IBaseComponent
{
	void setup(glm::vec3 _position = glm::vec3(1.0f), glm::vec3 _eulerAngles = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
	glm::vec3 position;
	glm::vec3 euler_angles;
	glm::vec3 scale;

	glm::mat4 model_matrix() const { return this->model_matrix_; }
	glm::quat rotation() const { return this->rotation_internal_; }
	glm::vec3 forward() const { return this->forward_; }
	glm::vec3 right() const { return this->right_; }
	glm::vec3 up() const { return this->up_; }
private:
	friend class MovementSystem;
	//gets calculated when movement system calls update() on this
	glm::mat4 model_matrix_;
	//gets calculated when movement system calls update() on this
	glm::quat rotation_internal_;
	//gets calculated when movement system calls update() on this
	glm::vec3 forward_;
	//gets calculated when movement system calls update() on this
	glm::vec3 right_;
	//gets calculated when movement system calls update() on this
	glm::vec3 up_;

};

#endif