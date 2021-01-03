#include "Entity.h"
#include "Component.h"
#include <exception>
#include <string>
#include <iostream>

namespace chrono
{

void Entity::Tick()
{
	for (const auto& component : components)
	{
		component.second->Tick();
	}
}

} //namespace chrono