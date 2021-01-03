#pragma once
#include <memory>
#include "Exception.h"
#include <iostream>

namespace chrono
{
class Entity;
class Core;

class Component {
	friend class Core;
	friend class Entity;
	


public:
	Component() {}
	~Component() {}
	Component(const Component&) = delete; //TODO
	Component operator=(const Component&) = delete; //TODO

	bool operator==(const Component& other) const
	{
		try {
			return (this->id == other.id);
			throw Exception("CHRONO::COMPONENT::EQUALITY_SIGN::Trying to compare a component with a non-component.");
		}
		catch (Exception& e)
		{
			std::cout << e.what();
		}
	}

	virtual void onInitialize() {}
	virtual void Tick() const {}

	std::shared_ptr<Core> GetCore() { return this->core.lock(); }
	std::shared_ptr<Entity> GetEntity() { return this->entity.lock(); }
	char GetType() { return this->id; }

protected:
	char id;
	enum componentID : char {
		Transform = 0,
		Movable = 1,
		Renderable = 2,
	};
	std::weak_ptr<Component> self;
	std::weak_ptr<Entity> entity;
	std::weak_ptr<Core> core;
	
};



} //namespace chrono