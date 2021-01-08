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



class ComponentManager;
class IBaseComponentTypeManager;
template<typename T>
class ComponentTypeManager;
class EntityManager;


struct TransformComponent;


class Core
{
	std::shared_ptr<EntityManager> entity_manager_;
	std::shared_ptr<ComponentManager> component_manager_;
	friend class Entity;
	friend class EntityManager;
public:
	Core() = default;

	static std::shared_ptr<Core> create_core()
	{
		return std::make_shared<Core>();
	}

	void start();
	void loop();

};

#endif
