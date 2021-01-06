#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <bitset>
#include <utility>



class EntityManager;

template<typename T>
class ComponentTypeManager;

class TransformComponent;
struct ComponentBitMask;

class Core
{
	std::shared_ptr<EntityManager> entity_manager_;
	std::shared_ptr<ComponentTypeManager<TransformComponent>> transform_manager_;

	friend class Entity;
	friend class EntityManager;
public:
	Core() = default;

	static std::shared_ptr<Core> create_core()
	{
		return std::make_shared<Core>();
	}

	void setup();


};

