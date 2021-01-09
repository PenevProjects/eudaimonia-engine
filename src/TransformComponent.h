#ifndef _TRANSFORM_COMPONENT_H
#define _TRANSFORM_COMPONENT_H


#include "Entity.h"


struct TransformComponent : public IBaseComponent
{
	int x_;
	int b_;
	void setup(int a, int b) {
		a_ = a;
		b_ = b;
	}

};

#endif