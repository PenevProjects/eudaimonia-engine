#ifndef _ENTITY_H
#define _ENTITY_H

#include <memory>
#include <vector>
#include <queue>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <bitset>
#include <utility>
#include <typeinfo>
#include <typeindex>

#include "Component.h"


#define MAX_COMPONENT_TYPES 1024
typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask;

class EntityManager;

template<typename T>
class ComponentTypeManager;


class Entity
	: public std::enable_shared_from_this<Entity>
{
	friend class EntityManager;
	template<typename T>
	friend class ComponentTypeManager;

	unsigned int id_;
	bool alive;

	std::shared_ptr<EntityManager> manager_;
	ComponentMask component_mask_;
public:
	Entity(std::weak_ptr<EntityManager> em, unsigned id) : manager_(em), id_(id), alive(true) {}

	unsigned id() { return this->id_; }

	bool operator == (const Entity &other) const { return id_ == other.id_; }
	bool operator != (const Entity &other) const { return id_ != other.id_; }

	void destroy();
	
	template<typename T, typename ... Args>
	std::shared_ptr<T> add_component(Args&& ... args)
	{
		auto component_instance = manager_->add_component<T>(this);
		//setup component
		component_instance->setup(std::forward<Args>(args)...);
		return component_instance;
	}

	template<typename T>
	std::shared_ptr<T> get_component()
	{
		return manager_->get_component<T>(this);
	}

	template<typename T>
	void remove_component()
	{
		manager_->remove_component<T>(this);
	}


};


class IBaseComponentTypeManager {
public:
	IBaseComponentTypeManager() = default;
	virtual ~IBaseComponentTypeManager() = default;
	IBaseComponentTypeManager(const IBaseComponentTypeManager &) = default;
	IBaseComponentTypeManager &operator=(const IBaseComponentTypeManager &) = default;
	IBaseComponentTypeManager(IBaseComponentTypeManager &&) = default;
	IBaseComponentTypeManager &operator=(IBaseComponentTypeManager &&) = default;
};


class ComponentManager
	: public std::enable_shared_from_this<ComponentManager>
{
	template<typename T>
	friend class ComponentTypeManager;
	friend class EntityManager;

	std::map<std::type_index, unsigned int> class_to_componentType;


	//map with component type managers of various types
	//key is the component type id
	std::map<unsigned int, std::shared_ptr<IBaseComponentTypeManager>> component_type_managers_;
	//component bit mask - gets populated from component type managers
	ComponentMask component_type_mask_;
public:

	ComponentManager() : component_type_mask_(0) {}

	template<typename T_ComponentType>
	std::shared_ptr<ComponentTypeManager<T_ComponentType>> create_ComponentTypeManager()
	{
		unsigned int id = MAX_COMPONENT_TYPES + 1;
		//assign id to the componentFamily
		for (size_t i = 0; i < component_type_mask_.size(); i++)
		{
			if (!component_type_mask_.test(i))
			{
				component_type_mask_.set(i);
				id = i;
				break;
			}
		}
		if (id == MAX_COMPONENT_TYPES + 1)
		{
			std::cout << "COMPONENTMANAGER::ERROR::maximum component types count reached, currently: " << MAX_COMPONENT_TYPES << std::endl;
		}

		auto created = std::make_shared<ComponentTypeManager<T_ComponentType>>(shared_from_this(), id);
		component_type_managers_.emplace(id, created);
		class_to_componentType.emplace(std::type_index(typeid(T_ComponentType)), id);
		return created;
	}
};






template<typename T_ComponentObject>
class ComponentTypeManager : public IBaseComponentTypeManager {
	friend class ComponentManager;
	friend class Entity;
	friend class EntityManager;

	/** A pair which is used to map Entity instances to component instances
	* This identifier type allows for duplicate component types attached to an entity
	*
	* @param Entity Entity owner of component type
	* @param unsigned_int The component family index on the entity's component_mask
	**/
	typedef std::pair<Entity*, unsigned int> ComponentIdentifier;


	/** Component container.
	*
	*
	*
	**/
	std::map<ComponentIdentifier, std::shared_ptr<T_ComponentObject>> component_instances_;


	std::shared_ptr<ComponentManager> component_manager_;
	unsigned int component_type_id_;


	/** Internal functions for adding component.
	*
	* The user can only call add component from entities.
	*/
private:
	template<typename ... Args>
	std::shared_ptr<T_ComponentObject> add_component_internal(Entity* entity)
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



		return componentObject;
	}


	std::shared_ptr<T_ComponentObject> get_component_internal(Entity* entity) const
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

	void remove_component_internal(Entity* entity)
	{
		if (entity->component_mask_.test(component_type_id_))
		{
			ComponentIdentifier instance{ entity, component_type_id_ };
			component_instances_.erase(instance);
			entity->component_mask_.reset(component_type_id_);
		}
		else
		{
			std::cout << "REMOVE_COMPONENT::ERROR::There's no such component attached to entity with id " << entity->id();
		}
	}


public:
	ComponentTypeManager(std::shared_ptr<ComponentManager> cm, unsigned int id)
		: component_manager_(cm),
		component_type_id_(id)
	{}
};









class EntityManager
	: public std::enable_shared_from_this<EntityManager>
{
	friend class Entity;
	friend class ComponentManager;

	template<typename T>
	friend class ComponentTypeManager;

	//entities vector
	std::vector<std::shared_ptr<Entity>> entities_;

	std::shared_ptr<ComponentManager> component_manager_;
	//maximum entity count is capacity of unsigned int
	//free_id_ gets iterated every time an entity gets created
	unsigned int free_id_;
	//queue for reusable ids
	//after an entity gets destroyed, its id can be reused
	std::queue<unsigned int> reusable_ids_;


public:

	EntityManager(const std::shared_ptr<ComponentManager>& cm)
		: free_id_(0),
		component_manager_(cm)
	{
	}

	/**	 Creates an Entity!
	*
	*
	*
	*
	*
	**/
	std::shared_ptr<Entity> create_entity()
	{
		unsigned int id;
		try {
			if (free_id_ == 0xffffffffUL)
			{
				throw std::exception("Maximum entity count reached!");
				free_id_--;
			}
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			return nullptr;
		}

		if (!reusable_ids_.empty())
		{
			id = reusable_ids_.front();
			reusable_ids_.pop();
		}
		else
		{
			id = free_id_;
		}

		auto newEntity = std::make_shared<Entity>(shared_from_this(), free_id_);
		entities_.push_back(newEntity);
		free_id_++;
		return newEntity;
	}

	void destroy(Entity* entity)
	{
		reusable_ids_.push(entity->id_);
		entities_.erase(entities_.begin() + entity->id_);
		entity->alive = false;
	}

	inline unsigned int entities_count() const { return entities_.size(); }

private:
	template<typename T>
	std::shared_ptr<T> get_component(Entity* entity)
	{
		//get componenttype from type of T
		unsigned int id = component_manager_->class_to_componentType.at(std::type_index(typeid(T)));
		auto type_manager = std::dynamic_pointer_cast<ComponentTypeManager<T>>(component_manager_->component_type_managers_.at(id));
		if (!type_manager)
		{
			std::cout << "GET_COMPONENT::ERROR::Couldn't get type of ComponentTypeManager from ComponentManager. Returning a nullptr." << std::endl;
			return nullptr;
		}
		return type_manager->get_component_internal(entity);
	}

	template<typename T>
	std::shared_ptr<T> add_component(Entity* entity)
	{
		unsigned int id = component_manager_->class_to_componentType.at(std::type_index(typeid(T)));
		auto type_manager = std::dynamic_pointer_cast<ComponentTypeManager<T>>(component_manager_->component_type_managers_.at(id));
		return type_manager->add_component_internal(entity);
	}

	template<typename T>
	void remove_component(Entity* entity)
	{
		unsigned int id = component_manager_->class_to_componentType.at(std::type_index(typeid(T)));
		auto type_manager = std::dynamic_pointer_cast<ComponentTypeManager<T>>(component_manager_->component_type_managers_.at(id));
		type_manager->remove_component_internal(entity);
	}

};


inline void Entity::destroy()
{
	manager_->destroy(this);
}

#endif