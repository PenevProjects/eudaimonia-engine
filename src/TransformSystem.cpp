#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "TransformSystem.h"
#include "Transform.h"
#include "zero/Entity.h"


void TransformSystem::buildModelMatrix(std::shared_ptr<Transform> transform)
{
	glm::mat4 translation_matrix = glm::translate(glm::mat4{ 1.0f }, transform->position);
	transform->rotation_internal_ = glm::quat(transform->rotation_euler);
	glm::mat4 rotation_matrix = glm::toMat4(transform->rotation_internal_);
	glm::mat4 scale_matrix = glm::scale(glm::mat4{ 1.0f }, transform->scale);
	transform->model_matrix_ = { translation_matrix * rotation_matrix * scale_matrix };
}

void TransformSystem::update(std::shared_ptr<Transform> transform)
{
	buildModelMatrix(transform);
	transform->forward_ = transform->rotation_internal_ * glm::vec3(0.0f, 0.0f, -1.0f);
	transform->right_ = transform->rotation_internal_ * glm::vec3(1.0f, 0.0f, 0.0f);
	transform->up_ = transform->rotation_internal_ * glm::vec3(0.0f, 1.0f, 0.0f);
}


void TransformSystem::tick()
{
	//TODO: this is too slow
	auto transform_manager = system_manager()->component_manager()->getTypeManager<Transform>();
	
	for (auto& instance : *transform_manager->instances_ptr())
	{
		auto transform = std::dynamic_pointer_cast<Transform>(instance);
		update(transform);
	}
}