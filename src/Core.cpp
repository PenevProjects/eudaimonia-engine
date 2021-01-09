#include "Core.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "System.h"


void Core::start()
{
	component_manager_ = ComponentManager::createComponentManager();
	entity_manager_ = EntityManager::createEntityManager(component_manager_);
	component_manager_->createComponentType<TransformComponent>();
	systems_manager_ = SystemsManager::createSystemsManager(entity_manager_, component_manager_);

	Entity* entity = entity_manager_->createEntity();
	Entity* entity2 = entity_manager_->createEntity();
	Entity* entity3 = entity_manager_->createEntity();
	entity3->addComponent<TransformComponent>(99, 99);
	auto component = entity->addComponent<TransformComponent>(5, 6);
	entity3->copyComponent<TransformComponent>(entity);


	auto copied = entity3->getComponent<TransformComponent>();
	copied->a_ = 1212;

	auto cc = entity->addComponent<TransformComponent>(5, 1);

	entity->destroy();

	auto c2 = entity->addComponent<TransformComponent>(5, 1);

	entity_manager_->deleteDeadEntities();
}

void Core::loop()
{




	systems_manager_->tickAll();
}
