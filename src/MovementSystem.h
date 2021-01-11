#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


#include "zero/System.h"

struct Transform;

class MovementSystem : public zero::IBaseSystem
{
public:
	void tick();
	void update(std::shared_ptr<Transform> transform);
	glm::mat4 buildModelMatrix(std::shared_ptr<Transform> transform);

	//void Rotate(glm::vec3 _eulerAngles);
	//void Scale(glm::vec3 _Scale);
};
