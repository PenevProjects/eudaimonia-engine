#include "Core.h"
#include "Component.h"
#include "Entity.h"

void Core::setup()
{
	entity_manager_ = std::make_shared<EntityManager>();
	transform_manager_ = std::make_shared<ComponentTypeManager<TransformComponent>>(entity_manager_);


	std::shared_ptr<Entity> entity = entity_manager_->create_entity();
	std::shared_ptr<Entity> entity2 = entity_manager_->create_entity();
	std::shared_ptr<Entity> entity3 = entity_manager_->create_entity();
	std::shared_ptr<TransformComponent> component = transform_manager_->add_component(entity, 5, 6);
	std::shared_ptr<TransformComponent> component1 = transform_manager_->add_component(entity2, 8, 8);
	std::shared_ptr<TransformComponent> component2 = transform_manager_->add_component(entity, 5, 6);
	std::shared_ptr<TransformComponent> component3 = transform_manager_->add_component(entity, 5, 6);
	auto tr = transform_manager_->get_component(entity);
	auto tr2 = transform_manager_->get_component(entity3);
}