#ifndef ZERO_CORE_H
#define ZERO_CORE_H
#include <memory>
#include <vector>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <bitset>
#include <utility>

#include "Entity.h"
#include "System.h"
namespace zero
{

class EntityManager;
class ComponentManager;
class SystemsManager;

class Core
{
	std::shared_ptr<EntityManager> entity_manager_;
	std::shared_ptr<ComponentManager> component_manager_;
	std::shared_ptr<SystemsManager> systems_manager_;
public:
	Core() = default;

	inline static std::unique_ptr<Core> createCore()
	{
		auto core = std::make_unique<Core>();
		core->start();
		return core;
	}
	inline EntityManager* entity_manager()
	{
		return entity_manager_.get();
	}
	inline ComponentManager* component_manager()
	{
		return component_manager_.get();
	}
	inline SystemsManager* systems_manager()
	{
		return systems_manager_.get();
	}
private:
	inline void start()
	{
		component_manager_ = ComponentManager::createComponentManager();
		entity_manager_ = EntityManager::createEntityManager(component_manager_);
		systems_manager_ = SystemsManager::createSystemsManager(entity_manager_, component_manager_);
	}
};
}//namespace zero
#endif
