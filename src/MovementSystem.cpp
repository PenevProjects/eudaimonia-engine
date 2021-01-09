#include "MovementSystem.h"
#include "Transform.h"
#include "zero/Entity.h"


glm::mat4 MovementSystem::getModelMatrix(Transform* transform) const
{
	glm::mat4 model { 1.0f };

	model = glm::translate(model, transform->position);
	model = glm::rotate(model, transform->orientation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, transform->orientation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, transform->orientation.z, glm::vec3(0, 0, 1));
	model = glm::scale(model, transform->scale);

	return model;
}

void MovementSystem::tick()
{
	auto transforms = manager_.lock()->component_manager()->getTypeManager<Transform>();
}
void MovementSystem::update()
{

}

//void Rotate(glm::vec3 _eulerAngles);
//void Scale(glm::vec3 _Scale);