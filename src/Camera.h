#ifndef _CAMERA_H
#define _CAMERA_H

#include "zero/Component.h"

struct Camera : public zero::IBaseComponent
{
public:
	float fov_;
public:
	///Creates Camera object with default values
	Camera() = default;
	void setup(float fov = 45.0f) { this->fov_ = fov; }
};

#endif
