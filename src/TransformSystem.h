#ifndef _TRANSFORM_SYSTEM_H
#define _TRANSFORM_SYSTEM_H

#include <memory>

#include "zero/System.h"

struct Transform;

class TransformSystem : public zero::IBaseSystem
{
public:
	void tick();
	void update(std::shared_ptr<Transform> transform);
	void buildModelMatrix(std::shared_ptr<Transform> transform);
};
#endif