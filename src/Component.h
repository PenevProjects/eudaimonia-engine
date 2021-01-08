#ifndef _COMPONENT_H
#define _COMPONENT_H

/** 
 * Inherit from this class to create components.
 *
 * All user-defined component types *must* have default constructors.
 * Initialization happens in "setup()".
 *
 * Purely abstract interface which implements the function "setup()" which is called when adding components to an entity.
 */
struct IBaseComponent
{
	// All derived component types *must* have default constructors
	IBaseComponent() = default;
	virtual ~IBaseComponent() = default;
	// Use this for initialization of components 
	virtual void setup() {}
};

struct TransformComponent : public IBaseComponent
{
	int a_;
	int b_;
	void setup(int a, int b) {
		a_ = a;
		b_ = b;
	}

};

#endif