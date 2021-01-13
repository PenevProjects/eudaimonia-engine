#ifndef _INPUT_SYSTEM_H
#define _INPUT_SYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
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
	float delta_x() { return this->delta_x_; }
	float delta_y() { return this->delta_y_; }
	int x() { return this->x_; }
	int y() { return this->y_; }
	/**
	 * Transforms the passed transform according to mouse motion.
	 */
	void controller(std::shared_ptr<Transform> transform, float sensitivity);

};



class InputSystem : public zero::IBaseSystem
{
	std::unique_ptr<Keyboard> keyboard_;
	std::unique_ptr<Mouse> mouse_;
public:
	std::vector<std::pair<std::shared_ptr<Transform>, float>> mouse_controllers_;
	bool quit_flag_;
	void setup();
	void tick();
	void update() {}
	void controlFromKeyboard(std::shared_ptr<Transform> transform, float speed);
	void attachMouseController(std::shared_ptr<Transform> transform, float sensitivity);
	Keyboard* keyboard() { return keyboard_.get(); };
	Mouse* mouse() { return mouse_.get(); }

};




#endif