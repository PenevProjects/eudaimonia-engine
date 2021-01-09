#ifndef _TRANSFORM_COMPONENT_H
#define _TRANSFORM_COMPONENT_H


#include "Component.h"

namespace zero
{

struct TransformComponent : public IBaseComponent
{
	int a_;
	int b_;
	void setup(int a, int b) {
		a_ = a;
		b_ = b;
	}

};
}//namespace zero

#endif