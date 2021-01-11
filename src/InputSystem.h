#ifndef _INPUT_SYSTEM_H
#define _INPUT_SYSTEM_H

#include "zero/System.h"

class InputSystem : public zero::IBaseSystem
{
	void ProcessKeyboardInput();
	void ProcessMouseInput(float xoffset, float yoffset);
};




#endif