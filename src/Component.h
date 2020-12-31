#pragma once
#include <memory>

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
	virtual void onInitialize() {}
	virtual void Tick() const {}
	std::shared_ptr<Entity> GetEntity() { return this->entity.lock(); }
private:
	std::weak_ptr<Component> self;
	std::weak_ptr<Entity> entity;
	std::weak_ptr<Core> core;
};



} //namespace chrono