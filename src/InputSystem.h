#ifndef _INPUT_SYSTEM_H
#define _INPUT_SYSTEM_H

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include "zero/System.h"
#include "Time.h"

struct Transform;


class Keyboard
{
	const Uint8* keyboard_state_;
public:
	void update();
	bool keyPressed(SDL_Scancode key);
};

class Mouse
{
	int delta_x_;
	int delta_y_;
	int x_;
	int y_;
public:
	void update();
	int delta_x() { return this->delta_x_; }
	int delta_y() { return this->delta_y_; }
	int x() { return this->x_; }
	int y() { return this->y_; }
	/**
	 * Transforms the passed transform according to mouse motion.
	 */
	void controller(std::shared_ptr<Transform> transform, float sensitivity);

};


/** \brief Input System
 *
 * Holds a reference to the keyboard and mouse.
 **/
class InputSystem : public zero::IBaseSystem
{
	//TODO: Keyboard handling needs to be improved
	std::unique_ptr<Keyboard> keyboard_;
	//TODO: Mouse handling needs to be improved
	std::unique_ptr<Mouse> mouse_;
public:
	std::vector<std::pair<std::shared_ptr<Transform>, float>> mouse_controllers_;
	bool quit_flag_;
	void setup();
	void tick();
	void update() {}
	// Called in the main game loop. Move with WASD.
	void controlFromKeyboard(std::shared_ptr<Transform> transform, float speed);
	// Uses relative motion to look around.
	void attachMouseController(std::shared_ptr<Transform> transform, float sensitivity);
	Keyboard* keyboard() { return keyboard_.get(); };
	Mouse* mouse() { return mouse_.get(); }

};




#endif