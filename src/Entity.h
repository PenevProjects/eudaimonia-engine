#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <exception>
#include <string>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "Exception.h"

namespace chrono 
{
class Core;
class Transform;
class Component;

class Entity
{
	friend class Core;
	friend class Component;
public:
	Entity() {}
	Entity(const Entity&) = delete; //TODO
	Entity operator=(const Entity&) = delete; //TODO

	template <typename T>
	std::shared_ptr<T> AddComponent()
	{
		std::shared_ptr<T> componentObj;
		try
		{
			componentObj = std::make_shared<T>();
			if (!componentObj)
				throw Exception("Unsuccessful AddComponent. AddComponent<T> expects a Component type T.");
		}
		catch (Exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		componentObj->self = componentObj;
		componentObj->entity = m_self;
		componentObj->core = this->GetCore();

		components[std::type_index(typeid(*Component))]{ componentObj };;

		componentObj->onInitialize();

		return componentObj;
	}

	template <typename T, typename ... Args>
	std::shared_ptr<T> AddComponent(Args&&... args)
	{
		std::shared_ptr<T> componentObj;
		componentObj = std::make_shared<Component>();
		if (!componentObj) { std::cout << "This component couldn't be added."; << std::endl; }

		componentObj->self = componentObj;
		componentObj->entity = m_self;
		componentObj->core = this->GetCore();

		components[std::type_index(typeid(*Component))]{ componentObj };

		componentObj->onInitialize(std::forward<Args>(args)...);

		return componentObj;
	}

	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		std::type_index index{ typeid(T) };
		if (components.count(std::type_index(typeid(T)) != 0))
		{
			return static_pointer_cast<T>(components[index]);
		}
		else
		{
			std::cout << "No such component attached to this entity.";
		}
		return nullptr;
	}

	void Tick();
	std::shared_ptr<Core> GetCore() { return m_core.lock(); }
private:
	std::weak_ptr<Entity> m_self;
	std::weak_ptr<Core> m_core;
	std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
	std::shared_ptr<Transform> transform;
};

} //namespace chrono