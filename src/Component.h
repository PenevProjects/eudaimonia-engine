#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <bitset>
#include <utility>
#include <any>


#include "Entity.h"
#include "Core.h"


/** All components must inherit from this
*
**/
class IBaseComponent
{
public:
	virtual ~IBaseComponent() = default;
	virtual void setup() {}
};

class TransformComponent : public IBaseComponent
{
	int a;
	int b;
public:
	TransformComponent() = default;
	void setup(int _a, int _b) {
		a = std::move(_a);
		b = std::move(_b);
	}

};


template<typename T_ComponentObject>
class ComponentTypeManager {
	/** A pair which is used to map Entity instances to component instances
	* This identifier type allows for duplicate component types attached to an entity
	*
	* @param Entity Entity owner of component type
	* @param unsigned_int The component family index on the entity's component_mask
	**/
	typedef std::pair<std::shared_ptr<Entity>, unsigned int> ComponentIdentifier;


	/** Component container.
	*
	*
	*
	**/
	std::map<ComponentIdentifier, std::shared_ptr<T_ComponentObject>> component_instances_;

	std::shared_ptr<ComponentMask> component_type_id_mask;
	unsigned int component_type_id_;

public:
	ComponentTypeManager(std::shared_ptr<EntityManager> em)
		: component_type_id_mask(em->component_bitmask_)
	{
		//assign id to the componentFamily
		for (size_t i = 0; i < component_type_id_mask->size(); i++)
		{
			if (!component_type_id_mask->test(i))
			{
				component_type_id_mask->set(i);
				component_type_id_ = i;
				break;
			}
			else
			{
				//TODO
				std::cout << "Max component types exceeded. This will lead to undefined behaviour." << std::endl;
			}
		}
	}




	template<typename ... Args>
	std::shared_ptr<T_ComponentObject> add_component(std::shared_ptr<Entity> entity, Args&& ... args)
	{
		ComponentIdentifier newComponentInstance = std::make_pair(entity, component_type_id_);


		if (entity->component_mask_.test(component_type_id_))
		{
			std::cout << "ADD_COMPONENT::ERROR::There is already a component of " << typeid(T_ComponentObject).name() << " attached to entity with id " << entity->id() << std::endl;
			return nullptr;
		}
		else
		{
			entity->component_mask_.set(component_type_id_);
		}

		std::shared_ptr<T_ComponentObject> componentObject = std::make_shared<T_ComponentObject>();

		//update components vector
		component_instances_.emplace(newComponentInstance, componentObject);


		//setup component
		componentObject->setup(std::forward<Args>(args)...);

		return componentObject;
	}


	std::shared_ptr<T_ComponentObject> get_component(std::shared_ptr<Entity> entity) const
	{ 
		if (entity->component_mask_.test(component_type_id_))
		{
			return component_instances_.at({ entity, component_type_id_ });
		}
		else
		{
			std::cout << "GET_COMPONENT::ERROR::There's no such component attached to entity with id " << entity->id();
			return nullptr;
		}
		
	}

	void remove_component(std::shared_ptr<Entity> entity)
	{
		if (entity->component_mask_.test(component_type_id_))
		{
			component_instances_.erase({ entity, component_type_id_ });
			entity->component_mask_.reset(component_type_id_);
		}
		else
		{
			std::cout << "REMOVE_COMPONENT::ERROR::There's no such component attached to entity with id " << entity->id();
		}
	}
};