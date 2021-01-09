#ifndef _CORE_H
#define _CORE_H
#include <memory>
#include <vector>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <bitset>
#include <utility>



class IBaseComponentTypeManager;
template<typename T_ComponentInstance>
class ComponentTypeManager;

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

	static std::unique_ptr<Core> CreateCore()
	{
		return std::make_unique<Core>();
	}

	void start();
	void loop();
};

#endif
