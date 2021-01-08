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



/**
 * A handle class for an entity id.
 *
 * Can be used 
 */
class Entity
{
	friend class EntityManager;
	template<typename T>
	friend class ComponentTypeManager;

	// index of entity
	unsigned int id_;
	// state of entity
	bool alive_;

	// Pointer to manager which handles add, get and remove component functions.
	std::weak_ptr<EntityManager> manager_;
	// bitset with each bit corresponding to a component type.
	// if a bit is 1, a component of that type is attached to the entity.
	ComponentMask component_mask_;
public:
	Entity(std::weak_ptr<EntityManager> em, unsigned id) : manager_(em), id_(id), alive_(true) {}

	//delete copy ctors to ensure no mismanagement happens.
	Entity (const Entity&) = delete;
	Entity operator=(const Entity&) = delete;

	//getters for id and alive.
	unsigned id() { return this->id_; }
	bool alive() { return this->alive_; }
	
	//operator overloading for entity comparisons
	bool operator == (const Entity &other) const { return id_ == other.id_; }
	bool operator != (const Entity &other) const { return id_ != other.id_; }
	operator bool() const { return alive_; }

	// sets alive_ = false;
	void destroy();
	
	/**
	 * Adds a component to an entity and intializes it with the parameters passed.
	 *
	 * @return A pointer to the component instance.
	 *
	 * Component Type needs to inherit from IBaseComponent.
	 *
	 * Usage:
	 *
	 *		auto component = entity->add_component<TransformComponent>(args);
	 *		OR
	 *		TransformComponent* component = entity->add_component<TransformComponent>(args);
	 */
	template<typename T_ComponentType, typename ... Args>
	T_ComponentType* add_component(Args&& ... args)
	{
		if (!alive_) { std::cout << "Trying to add component to a dead entity. ID: " << id_ << std::endl; return nullptr; }
		auto component_instance = manager_.lock()->add_component<T_ComponentType>(this);
		//setup component if add_component is successful
		if (component_instance)
		{
			component_instance->setup(std::forward<Args>(args)...);
		}
		return component_instance;
	}

	/**
	 * Gets a component that is attached to an entity.
	 *
	 * @return A pointer to the component instance.
	 *
	 * Usage:
	 *
	 *		auto component = entity->get_component<TransformComponent>();
	 *		OR
	 *		TransformComponent* component = entity->get_component<TransformComponent>();
	 */
	template<typename T_ComponentType>
	T_ComponentType* get_component()
	{
		if (!alive_) { std::cout << "Trying to get component from a dead entity. ID: " << id_ << std::endl; return nullptr; }
		return manager_.lock()->get_component<T_ComponentType>(this);
	}

	/**
	 * Copies a component from a source entity into the caller.
	 *
	 * Usage:
	 *		entity_receiver->get_component<TransformComponent>(entity_giver);
	 */
	template<typename T_ComponentType>
	void copy_component(Entity* from)
	{
		if (!alive_) { std::cout << "Trying to copy component from a dead entity. ID: " << id_ << std::endl; return; }
		manager_.lock()->copy_component<T_ComponentType>(this, from);
	}
	/**
	 * Removes a component that is attached to an entity.
	 *
	 * Usage:
	 *
	 *		entity->remove_component<TransformComponent>();
	 */
	template<typename T_ComponentType>
	void remove_component()
	{
		if (!alive_) { std::cout << "Trying to remove component from a dead entity. ID: " << id_ << std::endl; return; }
		manager_.lock()->remove_component<T_ComponentType>(this);
	}

};



/**
 * Manages Component types. 
 *
 * All Component types need to be declared before they can be added to entities. 
 */
class ComponentManager
{
	template<typename T>
	friend class ComponentTypeManager;
	friend class EntityManager;



	/**
	 * map with component type managers of various types
	 * key is the component type index
	 * unique_ptrs because we need definitive ownership in this vector.
	 */
	std::map<std::type_index, std::unique_ptr<IBaseComponentTypeManager>> type_managers_;
	//component bit mask - gets populated with component type indices
	ComponentMask component_type_mask_;
	
	// helper map for indexing type_managers_ when given a component type id
	std::map<unsigned int, std::type_index> typeId_to_typename;

public:
	ComponentManager() : component_type_mask_(0){}
	static std::shared_ptr<ComponentManager> create_ComponentManager()
	{
		return std::make_shared<ComponentManager>();
	}

	IBaseComponentTypeManager* get_type_manager_from_id(unsigned int _id) const
	{
		auto tempId = typeId_to_typename.at(_id);
		return (type_managers_.at(tempId)).get();
	}

	template<typename T>
	ComponentTypeManager<T>* get_type_manager_from_typename() const
	{
		ComponentTypeManager<T>* manager = dynamic_cast<ComponentTypeManager<T>*>(type_managers_.at(std::type_index(typeid(T))).get());
		if (manager)
		{
			return manager;
		}
		else
		{
			std::cout << "GET_TYPE_MANAGER_FROM_TYPENAME::ERROR::Can't get ComponentTypeManager from typename." << std::endl;
			return nullptr;
		}
	}

	/** All user-defined component types *must* be declared by calling this function.
	*
	* Assigns an index for the components' bitmask.
	*/
	template<typename T_ComponentType>
	void declare_component_type()
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

		auto created = std::make_unique<ComponentTypeManager<T_ComponentType>>(id);
		type_managers_.emplace(std::type_index(typeid(T_ComponentType)), std::move(created));

		typeId_to_typename.emplace(id, std::type_index(typeid(T_ComponentType)));
	}
};




// Interface for Component Type Managers.
class IBaseComponentTypeManager {
public:
	IBaseComponentTypeManager() = default;
	virtual ~IBaseComponentTypeManager() = default;
	IBaseComponentTypeManager(const IBaseComponentTypeManager &) = default;
	IBaseComponentTypeManager &operator=(const IBaseComponentTypeManager &) = default;
	IBaseComponentTypeManager(IBaseComponentTypeManager &&) = default;
	IBaseComponentTypeManager &operator=(IBaseComponentTypeManager &&) = default;

	virtual void clean_entity(Entity* _e) {}
	virtual void copy_component_internal(Entity* current_entity, Entity* source_entity) {}
};

template<typename T_ComponentObject>
class ComponentTypeManager 
	: public IBaseComponentTypeManager 
{
	friend class ComponentManager;
	friend class Entity;
	friend class EntityManager;


	/** 
	 * Component instances container.
	 * Uses Entity pointers(don't need ownership, so can use raw pointers) as a key which identifies each instance based on its entity owner.
	 *
	 **/
	std::map<Entity*, T_ComponentObject> component_instances_;

	unsigned int component_type_id_;

	void clean_entity(Entity* _entity)
	{
		remove_component_internal(_entity);
	}

	/** Internal private functions for adding, getting, removing component of this type.
	 *
	 * The user can only operate on components from entity objects.
	 */
	template<typename ... Args>
	T_ComponentObject* add_component_internal(Entity* entity)
	{
		//if the component exists in the entity
		if (entity->component_mask_.test(component_type_id_))
		{
			std::cout << "ADD_COMPONENT::ERROR::There is already a component of " << typeid(T_ComponentObject).name() << " attached to entity with id " << entity->id() << std::endl;
			return nullptr;
		}
		else //add it
		{
			entity->component_mask_.set(component_type_id_);
		}

		//update components vector
		//construct the pair directly in the map using emplace
		component_instances_.emplace(entity, T_ComponentObject{});

		T_ComponentObject* componentPtr = &component_instances_.at(entity);


		return componentPtr;
	}


	T_ComponentObject* get_component_internal(Entity* entity)
	{
		//if the entity has the component
		if (entity->component_mask_.test(component_type_id_))
		{
			return &component_instances_.at(entity);
		}
		else
		{
			std::cout << "GET_COMPONENT::ERROR::There's no such component attached to entity with id " << entity->id();
			return nullptr;
		}

	}

	void copy_component_internal(Entity* current_entity, Entity* source_entity)
	{
		//if source entity doesn't have the component
		if (!source_entity->component_mask_.test(component_type_id_))
		{
			std::cout << "COPY_COMPONENT::ERROR::There's no such component attached to the source entity with id " << source_entity->id();
		}

		T_ComponentObject copy{ component_instances_.at(source_entity) };

		// if current entity doesn't have the component
		if (!current_entity->component_mask_.test(component_type_id_))
		{
			current_entity->component_mask_.set(component_type_id_);
		}
		else
		{
			component_instances_.erase(current_entity);
		}
		//construct the pair directly in the map using emplace
		component_instances_.emplace(current_entity, std::move(copy));

		T_ComponentObject* componentPtr = &component_instances_.at(current_entity);
	}

	void remove_component_internal(Entity* entity)
	{
		if (entity->component_mask_.test(component_type_id_))
		{
			component_instances_.erase(entity);
			entity->component_mask_.reset(component_type_id_);
		}
		else
		{
			std::cout << "REMOVE_COMPONENT::ERROR::There's no such component attached to entity with id " << entity->id();
		}
	}


public:
	ComponentTypeManager( unsigned int id)
		:	component_type_id_(id)
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
	std::map<unsigned int, std::unique_ptr<Entity>> entities_;

	std::shared_ptr<ComponentManager> component_manager_;
	//maximum entity count is capacity of unsigned int
	//free_id_ gets iterated every time an entity gets created
	unsigned int free_id_;
	//queue for reusable ids
	//after an entity gets destroyed, its id can be reused
	std::vector<unsigned int> reusable_ids_;

	std::vector<unsigned int> entities_to_be_destroyed;

public:
	EntityManager(std::shared_ptr<ComponentManager> cm)
		: free_id_(0),
		component_manager_(cm)
	{}
	static std::shared_ptr<EntityManager> create_EntityManager(std::shared_ptr<ComponentManager> cm)
	{
		return std::make_shared<EntityManager>(cm);
	}

	/**	 
	 * Creates a new Entity.
	 * @return A pointer to an entity object, which is owned by the EntityManager.
	 *
	 */
	Entity* create_entity()
	{
		unsigned int id;
		if (free_id_ == 0xffffffffUL)
		{
			std::cout << "Maximum entity count reached!" << std::endl;
			free_id_--;
			return nullptr;
		}

		if (!reusable_ids_.empty())
		{
			id = reusable_ids_.back();
			reusable_ids_.pop_back();
		}
		else
		{
			id = free_id_;
			free_id_++;
		}

		auto newEntity = std::make_unique<Entity>(weak_from_this(), id);
		entities_.emplace(id, std::move(newEntity));
		return entities_.at(id).get();
	}

	/** Returns an entity given its id.
	*
	* Avoid using as it needs to do a bounds check(quite slow).
	*/
	Entity* get_entity(unsigned int id)
	{
		Entity* entityPtr;
		try 
		{
			entityPtr = this->entities_.at(id).get();
		}
		catch (const std::out_of_range& e)
		{
			std::cout << "GET_ENTITY::ERROR::Trying to access an entity with an invalid ID. Error message: " << e.what() << std::endl;
			return nullptr;
		}
		return entityPtr;
	}

	Entity* copy_entity(Entity* _source)
	{
		auto new_entity = create_entity();
		for (unsigned int i = 0; i < _source->component_mask_.size(); i++)
		{
			// if component exists
			if (_source->component_mask_.test(i))
			{
				auto type_manager = component_manager_->get_type_manager_from_id(i);
				type_manager->copy_component_internal(new_entity, _source);
			}
		}
		return new_entity;
	}

	void destroy_entity(Entity* _entity)
	{
		_entity->alive_ = false;
		entities_to_be_destroyed.push_back(_entity->id_);
	}

	//function to be called at the end of game loop.
	void erase_dead_entities()
	{
		for (auto dead_entity_id : entities_to_be_destroyed)
		{
			//destroy attached components
			for (unsigned int i = 0; i < entities_.at(dead_entity_id)->component_mask_.size(); i++)
			{
				if (entities_.at(dead_entity_id)->component_mask_.test(i))
				{
					auto type_manager = component_manager_->get_type_manager_from_id(i);

					if (type_manager)
					{
						type_manager->clean_entity(entities_.at(dead_entity_id).get());
					}
				}
			}
			//delete entity
			entities_.erase(dead_entity_id);
			reusable_ids_.push_back(dead_entity_id);
		}
		entities_to_be_destroyed.clear();
	}

	inline unsigned int entities_count() const { return entities_.size(); }

private:
	template<typename T>
	T* add_component(Entity* entity)
	{
		auto type_manager = component_manager_->get_type_manager_from_typename<T>();
		//check if type_manager exists
		if (!type_manager)
		{
			std::cout << "GET_COMPONENT::ERROR::Couldn't get type of ComponentTypeManager of " << typeid(T).name() << " from ComponentManager. Are you sure you have declared the component type with ComponentManager.declare_component_type()?" << std::endl;
			return nullptr;
		}
		return type_manager->add_component_internal(entity);
	}

	template<typename T>
	T* get_component(Entity* entity)
	{
		auto type_manager = component_manager_->get_type_manager_from_typename<T>();
		//check if type_manager exists

		if (!type_manager)
		{
			std::cout << "GET_COMPONENT::ERROR::Couldn't get type of ComponentTypeManager of " << typeid(T).name() << " from ComponentManager. Are you sure you have declared the component type with ComponentManager.declare_component_type()?" << std::endl;
			return nullptr;
		}
		return type_manager->get_component_internal(entity);
	}

	template<typename T>
	void copy_component(Entity* current, Entity* source)
	{
		auto type_manager = component_manager_->get_type_manager_from_typename<T>();
		if (!type_manager)
		{
			std::cout << "GET_COMPONENT::ERROR::Couldn't get type of ComponentTypeManager of " << typeid(T).name() << " from ComponentManager. Are you sure you have declared the component type with ComponentManager.declare_component_type()?" << std::endl;
			return;
		}
		type_manager->copy_component_internal(current, source);
	}

	template<typename T>
	void remove_component(Entity* entity)
	{
		auto type_manager = component_manager_->get_type_manager_from_typename<T>();
		if (!type_manager)
		{
			std::cout << "GET_COMPONENT::ERROR::Couldn't get type of ComponentTypeManager of " << typeid(T).name() << " from ComponentManager. Are you sure you have declared the component type with ComponentManager.declare_component_type()?" << std::endl;
			return;
		}
		type_manager->remove_component_internal(entity);
	}

};


inline void Entity::destroy()
{
	manager_.lock()->destroy_entity(this);
}



#endif