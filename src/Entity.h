#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <bitset>
#include <utility>


#include "Component.h"
#include "Core.h"

#define MAX_COMPONENT_TYPES 64


typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask;


class EntityManager;
class Entity
{
	friend class EntityManager;
	template<typename T>
	friend class ComponentTypeManager;

	ComponentMask component_mask_;
	uint32_t id_;
	std::weak_ptr<EntityManager> manager_;

public:
	Entity(std::weak_ptr<EntityManager> em, uint32_t id) : id_(id), manager_(em) {}

	uint32_t id() { return this->id_; }
	
	template<typename T>
	std::shared_ptr<T> get_component()
	{
	}
};




class EntityManager
	: public std::enable_shared_from_this<EntityManager>
{
	friend class Entity;

	template<typename T>
	friend class ComponentTypeManager;

	//entities vector
	std::vector<std::shared_ptr<Entity>> entities_;

	//maximum entity count is capacity of uint32_t ~ 4.3 billion
	uint32_t used_ids_;
	std::shared_ptr<ComponentMask> component_bitmask_;


public:

	EntityManager() : used_ids_(0), component_bitmask_(std::make_shared<ComponentMask>()) {}
	
	/**	 Creates an Entity!
	*
	*
	*
	*
	*
	**/
	std::shared_ptr<Entity> create_entity()
	{
		try {
			if (used_ids_ == 0xffffffffUL)
			{
				throw std::exception("Maximum entity count reached!");
				used_ids_--;
			}
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			return nullptr;
		}
		auto newEntity = std::make_shared<Entity>(weak_from_this(), used_ids_);
		entities_.emplace_back(newEntity);
		used_ids_++;
		return newEntity;
	}

	uint32_t entities_count() const { return entities_.size(); }

};