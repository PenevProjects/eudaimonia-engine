#ifndef _TRANSFORM_COMPONENT_H
#define _TRANSFORM_COMPONENT_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <memory>

#include "zero/Component.h"


struct Transform : public zero::IBaseComponent
{
	void setup(glm::vec3 _position = glm::vec3(1.0f), glm::vec3 _orientation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 facing;
	glm::vec3 scale;
	glm::mat4 model;
};

#endif