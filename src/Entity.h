#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <exception>
#include <string>


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
				throw std::exception("Unsuccessful AddComponent. AddComponent<T> expects a Component type T.");
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		componentObj->self = componentObj;
		componentObj->entity = m_self;
		componentObj->core = this->GetCore();

		this->m_components.push_back(componentObj);

		componentObj->onInitialize();

		return componentObj;
	}

	template <typename T, typename ... Args>
	std::shared_ptr<T> AddComponent(Args&&... args)
	{
		std::shared_ptr<T> componentObj;
		try
		{
			componentObj = std::make_shared<T>();
			if (!componentObj)
				throw std::exception("Unsuccessful AddComponent. AddComponent<T> expects a Component class T.");
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		componentObj->self = componentObj;
		componentObj->entity = m_self;
		componentObj->core = this->GetCore();

		m_components.push_back(componentObj);

		componentObj->onInitialize(std::forward<Args>(args)...);

		return componentObj;
	}

	void Tick();
	std::shared_ptr<Core> GetCore() { return m_core.lock(); }
private:
	std::weak_ptr<Entity> m_self;
	std::weak_ptr<Core> m_core;
	std::vector<std::shared_ptr<Component>> m_components;
	std::shared_ptr<Transform> transform;
};

} //namespace chrono