#ifndef ZERO_COMPONENT_H
#define ZERO_COMPONENT_H

namespace zero
{
class Entity;
/**
 * Inherit from this class when creating custom components.
 *
 * All user-defined component types *must* have default constructors.
 * Initialization happens in the virtual function "setup()".
 *
 * Purely abstract interface which implements the function "setup()" which is called when adding components to an entity.
 */
class IBaseComponent
{
public:
	Entity* entity_owner_;
	// All derived component types *must* have default constructors
	IBaseComponent() = default;
	virtual ~IBaseComponent() = default;
	// Use this for initialization of components 
	virtual void setup() {}
};
}//namespace zero

#endif