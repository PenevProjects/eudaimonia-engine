#include "Core.h"
#include "Entity.h"
#include "Component.h"

void Core::setup()
{
	component_manager_ = std::make_shared<ComponentManager>();
	entity_manager_ = std::make_shared<EntityManager>(component_manager_);
	auto transform_manager_ = component_manager_->create_ComponentTypeManager<TransformComponent>();

	std::shared_ptr<Entity> entity = entity_manager_->create_entity();
	std::shared_ptr<Entity> entity2 = entity_manager_->create_entity();
	std::shared_ptr<Entity> entity3 = entity_manager_->create_entity();
	std::shared_ptr<TransformComponent> component = entity->add_component<TransformComponent>(5, 6);

	auto et = entity->get_component<TransformComponent>();

	entity->remove_component<TransformComponent>();


}