#include "Entity.h"
#include "Component.h"
#include <exception>
#include <string>
#include <iostream>

namespace chrono
{

void Entity::Tick()
{
	for (const auto& component : m_components)
	{
		component->Tick();
	}
}

} //namespace chrono