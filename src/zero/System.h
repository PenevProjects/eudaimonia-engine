#ifndef ZERO_SYSTEM_H
#define ZERO_SYSTEM_H
#include <memory>
#include <vector>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <bitset>
#include <typeindex>

#include "Entity.h"
#include "help/NonCopyable.h"


namespace zero
{
class SystemsManager;

// Interface for Systems.
class IBaseSystem 
{
	friend class SystemsManager;
	std::weak_ptr<SystemsManager> system_manager_;
public:
	IBaseSystem() = default;
	virtual ~IBaseSystem() = default;
	IBaseSystem(const IBaseSystem &) = default;
	IBaseSystem &operator=(const IBaseSystem &) = default;
	IBaseSystem(IBaseSystem &&) = default;
	IBaseSystem &operator=(IBaseSystem &&) = default;
	
	// called when system is added to a SystemsManager object.
	virtual void setup() {}
	// called every game loop tick
	virtual void tick() {}
	// called whenever invoked
	virtual void update() {}

	std::shared_ptr<SystemsManager> system_manager()
	{
		return system_manager_.lock();
	}
};



class SystemsManager : 
	public NonCopyable,
	public std::enable_shared_from_this<SystemsManager>
{
	friend class Core;
	friend class IBaseSystem;
	/**
	 * map with systems of various types. map is nice because we aren't going to be modifying the container.
	 * key is the system type index
	 * unique_ptrs because we need definitive ownership in this vector.
	 */
	std::map<std::type_index, std::shared_ptr<IBaseSystem>> systems_;
	std::weak_ptr<EntityManager> entity_manager_;
	std::weak_ptr<ComponentManager> component_manager_;

public:
	SystemsManager() = default;


	inline std::shared_ptr<ComponentManager> component_manager()
	{
		return component_manager_.lock();
	}
	inline std::shared_ptr<EntityManager> entity_manager()
	{
		return entity_manager_.lock();
	}
	template<typename T_System>
	std::shared_ptr<T_System> getSystem() const
	{
		std::shared_ptr<T_System> system = std::dynamic_pointer_cast<T_System>(systems_.at(std::type_index(typeid(T_System))));
		if (system)
		{
			return system;
		}
		else
		{
			std::cout << "GET_SYSTEM_FROM_TYPENAME::ERROR::Can't get ComponentTypeManager from typename." << std::endl;
			return nullptr;
		}
	}

	/** All user-defined component types *must* be declared by calling this function.
	*
	* Creates the System.
	*/
	template<typename T_System, typename ... Args>
	std::shared_ptr<T_System> addSystem(Args&& ... args)
	{
		std::shared_ptr<T_System> created = std::make_shared<T_System>();
		created->system_manager_ = weak_from_this();
		if (created)
		{
			created->setup(std::forward<Args>(args)...);
		}
		else
		{
			std::cout << "ADD_SYSTEM::ERROR::Could not create system. Please make sure this System derives from IBaseSystem and it has a constructor with no parameters." << std::endl;
		}

		systems_.emplace(std::type_index(typeid(T_System)), std::move(created));

		return getSystem<T_System>();
	}

	void tickAll()
	{
		for (auto& system : systems_)
		{
			system.second->tick();
		}
	}

private:
	inline static std::shared_ptr<SystemsManager> createSystemsManager(std::shared_ptr<EntityManager> _em, std::shared_ptr<ComponentManager> _cm)
	{
		std::shared_ptr<SystemsManager> obj = std::make_shared<SystemsManager>();
		obj->entity_manager_ = _em;
		obj->component_manager_ = _cm;
		return obj;
	}
};

}//namespace zero
#endif