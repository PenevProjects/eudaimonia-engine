#pragma once
#include <memory>
#include "Core.h"
#include "Component.h"

namespace chrono 
{

class Entity
{
public:
	Entity();
	~Entity();

	template <typename T, typename ... Args>
	std::shared_ptr<T> addComponent(Args&&... args);
private:
	std::weak_ptr<Entity> m_self;
	std::weak_ptr<Core> m_core;
	std::vector<Component> m_components;
};

} //namespace chrono