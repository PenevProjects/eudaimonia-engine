#include "Core.h"
#include "Entity.h"
#include "Component.h"


void Core::start()
{
	component_manager_ = ComponentManager::create_ComponentManager();
	entity_manager_ = EntityManager::create_EntityManager(component_manager_);
	component_manager_->declare_component_type<TransformComponent>();

	Entity* entity = entity_manager_->create_entity();
	Entity* entity2 = entity_manager_->create_entity();
	Entity* entity3 = entity_manager_->create_entity();
	auto component = entity->add_component<TransformComponent>(5, 6);
	entity3->copy_component<TransformComponent>(entity);


	auto copied = entity3->get_component<TransformComponent>();
	copied->a_ = 1212;

	auto cc = entity->add_component<TransformComponent>(5, 1);

	entity->destroy();

	auto c2 = entity->add_component<TransformComponent>(5, 1);

	entity_manager_->erase_dead_entities();
}

void Core::loop()
{




	entity_manager_->erase_dead_entities();
}
