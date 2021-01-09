#include "MovementSystem.h"
#include "Transform.h"
#include "zero/Entity.h"

//TODO Copy component and general "DoForAllInstances" function

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
	auto transform_manager = system_manager()->component_manager()->getTypeManager<Transform>();
	for (auto& instance : *transform_manager->instances_ptr())
	{
		auto transform = std::dynamic_pointer_cast<Transform>(instance);
		transform->model = getModelMatrix(transform.get());
	}
}
void MovementSystem::update()
{

}

//void Rotate(glm::vec3 _eulerAngles);
//void Scale(glm::vec3 _Scale);