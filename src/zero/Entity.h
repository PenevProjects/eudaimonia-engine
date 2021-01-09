#ifndef ZERO_ENTITY_H
#define ZERO_ENTITY_H

/**
 * Header-implemented because of heavy use of templates.
 */

#include <memory>
#include <vector>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <bitset>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <algorithm>


#include "help/NonCopyable.h"
#include "Component.h"


namespace zero
{
#define MAX_COMPONENT_TYPES 1024
typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask;

class EntityManager;
class Core;
template<typename T_ComponentInstance>
class ComponentTypeManager;

/**
 * A handle class for an entity id.
 *
 * Can be used 
 */
class Entity
{
	friend class EntityManager;
	template<typename T_ComponentInstance>
	friend class ComponentTypeManager;

	// index of entity
	unsigned int id_;
	// state of entity
	bool alive_;

	// Pointer to manager which handles add, get and remove component functions.
	std::weak_ptr<EntityManager> entity_manager_;
	// bitset with each bit corresponding to a component type.
	// if a bit is 1, a component of that type is attached to the entity.
	ComponentMask component_mask_;
public:
	Entity() = default;
	~Entity() = default;
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
	std::shared_ptr<T_ComponentType> addComponent(Args&& ... args)
	{
		if (!alive_) 
		{ 
			std::cout << "Trying to add component to a dead entity. ID: " << id_ << std::endl; 
			return nullptr; 
		}
		auto component_instance = entity_manager_.lock()->addComponent<T_ComponentType>(this);
		//setup component if add_component is successful
		if (component_instance)
		{
			component_instance->setup(std::forward<Args>(args)...);
		}
		return component_instance;
	}

	/**
	 * Gets a component that is attached to an entity.
	 * Very slow, O(n) speed, avoid using in the game loop.
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
	std::shared_ptr<T_ComponentType> getComponent()
	{
		if (!alive_) 
		{ 
			std::cout << "Trying to get component from a dead entity. ID: " << id_ << std::endl; 
			return nullptr; 
		}
		return entity_manager_.lock()->getComponent<T_ComponentType>(this);
	}

	/**
	 * Copies a component from a source entity into the caller.
	 * Very slow, avoid using in the game loop.
	 *
	 * Usage:
	 *		entity_receiver->get_component<TransformComponent>(entity_giver);
	 */
	template<typename T_ComponentType>
	void copyComponent(Entity* from)
	{
		if (!alive_) 
		{ 
			std::cout << "Trying to copy component from a dead entity. ID: " << id_ << std::endl;
			return; 
		}
		entity_manager_.lock()->copyComponent<T_ComponentType>(this, from);
	}
	/**
	 * Removes a component that is attached to an entity.
	 *
	 * Very slow, avoid using in the game loop.
	 * Usage:
	 *
	 *		entity->remove_component<TransformComponent>();
	 */
	template<typename T_ComponentType>
	void removeComponent()
	{
		if (!alive_) 
		{ 
			std::cout << "Trying to remove component from a dead entity. ID: " << id_ << std::endl;
			return;
		}
		entity_manager_.lock()->removeComponent<T_ComponentType>(this);
	}

};




// Interface for Component Type Managers.
class IBaseComponentTypeManager {
protected:
	std::weak_ptr<std::vector<std::shared_ptr<IBaseComponent>>> weak_to_instances_;
public:
	IBaseComponentTypeManager() = default;
	virtual ~IBaseComponentTypeManager() = default;
	IBaseComponentTypeManager(const IBaseComponentTypeManager &) = default;
	IBaseComponentTypeManager &operator=(const IBaseComponentTypeManager &) = default;
	IBaseComponentTypeManager(IBaseComponentTypeManager &&) = default;
	IBaseComponentTypeManager &operator=(IBaseComponentTypeManager &&) = default;

	virtual void cleanEntity(Entity* _e) {}
	virtual void copyComponentInternal(Entity* current_entity, Entity* source_entity) {}

	std::shared_ptr<std::vector<std::shared_ptr<IBaseComponent>>> instances_ptr()
	{
		return weak_to_instances_.lock();
	}
};


/**
 * Manages Component types. 
 *
 * All Component types need to be declared before they can be added to entities. 
 */
class ComponentManager : public NonCopyable
{
	template<typename T_ComponentInstance>
	friend class ComponentTypeManager;
	friend class EntityManager;
	friend class Core;



	/**
	 * map with component type managers of various types
	 * key is the component type index
	 * unique_ptrs because we need definitive ownership in this vector.
	 */
	std::map<std::type_index, std::shared_ptr<IBaseComponentTypeManager>> type_managers_;
	//component bit mask - gets populated with component type indices
	ComponentMask component_bitmask_;
	
	// helper map for indexing type_managers_ when given a component type id
	std::map<unsigned int, std::type_index> typeId_to_typename;

	//conceal because we want only the core to be able to create the component manager
	static std::shared_ptr<ComponentManager> createComponentManager()
	{
		return std::make_shared<ComponentManager>();
	}
public:
	ComponentManager() : component_bitmask_(0){}

	std::shared_ptr<IBaseComponentTypeManager> getTypeManager(unsigned int _id) const
	{
		auto tempId = typeId_to_typename.at(_id);
		return type_managers_.at(tempId);
	}

	template<typename T_ComponentInstance>
	std::shared_ptr<ComponentTypeManager<T_ComponentInstance>> getTypeManager() const
	{
		auto manager = std::dynamic_pointer_cast<ComponentTypeManager<T_ComponentInstance>>(type_managers_.at(std::type_index(typeid(T_ComponentInstance))));
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
	void createComponentType()
	{
		unsigned int id = MAX_COMPONENT_TYPES + 1;
		//assign id to the componentFamily
		for (size_t i = 0; i < component_bitmask_.size(); i++)
		{
			if (!component_bitmask_.test(i))
			{
				component_bitmask_.set(i);
				id = i;
				break;
			}
		}
		if (id == MAX_COMPONENT_TYPES + 1)
		{
			std::cout << "COMPONENTMANAGER::ERROR::maximum component types count reached, currently: " << MAX_COMPONENT_TYPES << std::endl;
		}

		auto created = std::make_shared<ComponentTypeManager<T_ComponentType>>();
		created->component_type_id_ = id;
		created->instances_ = std::make_shared<std::vector<std::shared_ptr<IBaseComponent>>>();
		created->weak_to_instances_ = created->instances_;

		type_managers_.emplace(std::type_index(typeid(T_ComponentType)), std::move(created));

		typeId_to_typename.emplace(id, std::type_index(typeid(T_ComponentType)));
	}
};


template<typename T_ComponentInstance>
class ComponentTypeManager 
	: public IBaseComponentTypeManager
{
private:
	friend class ComponentManager;
	friend class Entity;
	friend class EntityManager;
	/** 
	 * Component instances container.
	 **/
	std::shared_ptr<std::vector<std::shared_ptr<IBaseComponent>>> instances_;

	unsigned int component_type_id_;

	/** Internal private functions for adding, getting, removing component of this type.
	 *
	 * The user can only operate on components from entity objects.
	 */
	template<typename ... Args>
	std::shared_ptr<T_ComponentInstance> addComponentInternal(Entity* entity)
	{
		//if the component exists in the entity
		if (entity->component_mask_.test(component_type_id_))
		{
			std::cout << "ADD_COMPONENT::ERROR::There is already a component of " << typeid(T_ComponentInstance).name() << " attached to entity with id " << entity->id() << std::endl;
			return nullptr;
		}
		else //add it
		{
			entity->component_mask_.set(component_type_id_);
		}
		//update components vector
		auto obj = std::make_shared<T_ComponentInstance>();
		obj->entity_owner_ = entity;
		instances_->emplace_back(std::move(obj));
		return getComponentInternal(entity);
	}

	//Quite slow, avoid using too much.
	std::shared_ptr<T_ComponentInstance> getComponentInternal(Entity* entity)
	{
		//if the entity has the component
		if (entity->component_mask_.test(component_type_id_))
		{
			auto instance_itr = findComponentInstance(entity);
			if (instance_itr == instances_->end())
			{
				return nullptr;
			}
			return std::dynamic_pointer_cast<T_ComponentInstance>((*instance_itr));
		}
		else
		{
			std::cout << "GET_COMPONENT::ERROR::There's no such component attached to entity with id " << entity->id();
			return nullptr;
		}

	}

	void copyComponentInternal(Entity* current_entity, Entity* source_entity)
	{
		//if source entity doesn't have the component
		if (!source_entity->component_mask_.test(component_type_id_))
		{
			std::cout << "COPY_COMPONENT::ERROR::There's no such component attached to the source entity with id " << source_entity->id();
			return;
		}
		//find component instance attached to entity
		auto source_instance_itr = findComponentInstance(source_entity);
		//TODO:
		//TODO
		auto tempObj = std::make_shared<T_ComponentInstance>();
		tempObj->entity_owner_ = current_entity;
		// if current entity has the component
		if (current_entity->component_mask_.test(component_type_id_))
		{
			removeComponentInternal(current_entity);
		}
		current_entity->component_mask_.set(component_type_id_);
		instances_->emplace_back(std::move(tempObj));
	}

	void removeComponentInternal(Entity* entity)
	{
		if (entity->component_mask_.test(component_type_id_))
		{
			auto instance_itr = findComponentInstance(entity);
			instances_->erase(instance_itr);
			entity->component_mask_.reset(component_type_id_);
		}
		else
		{
			std::cout << "REMOVE_COMPONENT::ERROR::There's no such component attached to entity with id " << entity->id();
		}
	}

	//returns an iterator using std::find_if and reports if it doesn't exist.
	auto findComponentInstance(Entity* _entity)
	{
		auto instance_itr = std::find_if(instances_->begin(),
			instances_->end(),
			[&](const std::shared_ptr<IBaseComponent>& instance)
		{
			return instance->entity_owner_ == _entity;
		});

		if (instance_itr == instances_->end())
		{
			std::cout << "FIND_COMPONENT_INSTANCE::ERROR::Couldn't find given component instance. Check parameters. The return value of this function is invalid." << std::endl;
		}
		return instance_itr;
		
	}

	void cleanEntity(Entity* _entity)
	{
		removeComponentInternal(_entity);
	}
};

class EntityManager
	: public std::enable_shared_from_this<EntityManager>,
	public NonCopyable
{
	friend class Core;
	friend class Entity;
	friend class ComponentManager;
	template<typename T_ComponentInstance>
	friend class ComponentTypeManager;
	std::weak_ptr<ComponentManager> component_manager_;
	//entities vector
	std::map<unsigned int, std::unique_ptr<Entity>> entities_;
	//maximum entity count is capacity of unsigned int
	//free_id_ gets iterated every time an entity gets created
	unsigned int free_id_;
	//vector for reusable ids
	//after an entity gets destroyed, its id can be reused
	std::vector<unsigned int> reusable_ids_;
	// vector for entities to be destroyed,
	// fast access and contiguous memory
	std::vector<unsigned int> entities_to_be_destroyed;

public:
	EntityManager() = default;


	/**	 
	 * Creates a new Entity.
	 * @return A pointer to an entity object, which is owned by the EntityManager.
	 *
	 */
	Entity* createEntity()
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

		auto newEntity = std::make_unique<Entity>();
		newEntity->entity_manager_ = weak_from_this();
		newEntity->id_ = id;
		newEntity->alive_ = true;
		entities_.emplace(id, std::move(newEntity));
		return entities_.at(id).get();
	}

	/** Returns an entity given its id.
	*
	* Avoid using as it needs to do a bounds check(quite slow).
	*/
	Entity* getEntity(unsigned int id)
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

	Entity* copyEntity(Entity* _source)
	{
		auto new_entity = createEntity();
		for (unsigned int i = 0; i < _source->component_mask_.size(); i++)
		{
			// if component exists
			if (_source->component_mask_.test(i))
			{
				auto type_manager = component_manager_.lock()->getTypeManager(i);
				type_manager->copyComponentInternal(new_entity, _source);
			}
		}
		return new_entity;
	}

	void destroyEntity(Entity* _entity)
	{
		_entity->alive_ = false;
		entities_to_be_destroyed.push_back(_entity->id_);
	}

	//function to be called at the end of game loop.
	//cleans entity vector of dead entities
	void deleteDeadEntities()
	{
		for (auto dead_entity_id : entities_to_be_destroyed)
		{
			//destroy attached components
			for (unsigned int i = 0; i < entities_.at(dead_entity_id)->component_mask_.size(); i++)
			{
				if (entities_.at(dead_entity_id)->component_mask_.test(i))
				{
					auto type_manager = component_manager_.lock()->getTypeManager(i);

					if (type_manager)
					{
						type_manager->cleanEntity(entities_.at(dead_entity_id).get());
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
	inline void tick()
	{
		deleteDeadEntities();
	}

private:
	static std::shared_ptr<EntityManager> createEntityManager(std::shared_ptr<ComponentManager> cm)
	{
		auto obj = std::make_shared<EntityManager>();
		obj->component_manager_ = cm;
		obj->free_id_ = 0;
		return obj;
	}

	template<typename T_ComponentInstance>
	std::shared_ptr<T_ComponentInstance> addComponent(Entity* entity)
	{
		auto type_manager = component_manager_.lock()->getTypeManager<T_ComponentInstance>();
		//check if type_manager exists
		if (!type_manager)
		{
			std::cout << "GET_COMPONENT::ERROR::Couldn't get type of ComponentTypeManager of " << typeid(T_ComponentInstance).name() << " from ComponentManager. Are you sure you have declared the component type with ComponentManager.declare_component_type()?" << std::endl;
			return nullptr;
		}
		return type_manager->addComponentInternal(entity);
	}

	template<typename T_ComponentInstance>
	std::shared_ptr<T_ComponentInstance> getComponent(Entity* entity)
	{
		auto type_manager = component_manager_.lock()->getTypeManager<T_ComponentInstance>();
		//check if type_manager exists

		if (!type_manager)
		{
			std::cout << "GET_COMPONENT::ERROR::Couldn't get type of ComponentTypeManager of " << typeid(T_ComponentInstance).name() << " from ComponentManager. Are you sure you have declared the component type with ComponentManager.declare_component_type()?" << std::endl;
			return nullptr;
		}
		return type_manager->getComponentInternal(entity);
	}

	template<typename T_ComponentInstance>
	void copyComponent(Entity* current, Entity* source)
	{
		auto type_manager = component_manager_.lock()->getTypeManager<T_ComponentInstance>();
		if (!type_manager)
		{
			std::cout << "GET_COMPONENT::ERROR::Couldn't get type of ComponentTypeManager of " << typeid(T_ComponentInstance).name() << " from ComponentManager. Are you sure you have declared the component type with ComponentManager.declare_component_type()?" << std::endl;
			return;
		}
		type_manager->copyComponentInternal(current, source);
	}

	template<typename T_ComponentInstance>
	void removeComponent(Entity* entity)
	{
		auto type_manager = component_manager_.lock()->getTypeManager<T_ComponentInstance>();
		if (!type_manager)
		{
			std::cout << "GET_COMPONENT::ERROR::Couldn't get type of ComponentTypeManager of " << typeid(T_ComponentInstance).name() << " from ComponentManager. Are you sure you have declared the component type with ComponentManager.declare_component_type()?" << std::endl;
			return;
		}
		type_manager->remove_component_internal(entity);
	}

};


inline void Entity::destroy()
{
	entity_manager_.lock()->destroyEntity(this);
}
}//namespace zero
#endif