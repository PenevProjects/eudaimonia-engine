#include "TransformSystem.h"
#include "Transform.h"
#include "zero/Entity.h"

//TODO Copy component and general "DoForAllInstances" function

void TransformSystem::buildModelMatrix(std::shared_ptr<Transform> transform)
{
	glm::mat4 translation_matrix = glm::translate(glm::mat4{ 1.0f }, transform->position);
	transform->rotation_internal_ = glm::quat(transform->euler_angles);
	glm::mat4 rotation_matrix = glm::mat4_cast(transform->rotation_internal_);
	glm::mat4 scale_matrix = glm::scale(glm::mat4{ 1.0f }, transform->scale);
	transform->model_matrix_ = { translation_matrix * rotation_matrix * scale_matrix };
}

void TransformSystem::update(std::shared_ptr<Transform> transform)
{
	buildModelMatrix(transform);
	transform->forward_ = transform->rotation_internal_ * transform->forward_;
	transform->right_ = transform->rotation_internal_ * transform->right_;
	transform->up_ = transform->rotation_internal_ * transform->up_;
}


void TransformSystem::tick()
{
	//TODO
	auto transform_manager = system_manager()->component_manager()->getTypeManager<Transform>();
	
	for (auto& instance : *transform_manager->instances_ptr())
	{
		auto transform = std::dynamic_pointer_cast<Transform>(instance);
		update(transform);
	}
}