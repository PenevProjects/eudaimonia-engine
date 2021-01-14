#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "InputSystem.h"
#include "Transform.h"

void Keyboard::update()
{
	keyboard_state_ = SDL_GetKeyboardState(NULL);
}

bool Keyboard::keyPressed(SDL_Scancode key)
{
	return keyboard_state_[key] ? true : false;
}

void Mouse::update()
{
	SDL_GetMouseState(&x_, &y_);
	SDL_GetRelativeMouseState(&delta_x_, &delta_y_);
}

void Mouse::controller(std::shared_ptr<Transform> transform, float sensitivity)
{
	transform->rotation_euler.y += -delta_x_ * sensitivity * (float)Time::delta_time();
	transform->rotation_euler.x -= delta_y_ * sensitivity * (float)Time::delta_time();
	if (transform->rotation_euler.x > glm::radians(89.0f))
	{
		transform->rotation_euler.x = glm::radians(89.0f);
	}
	if (transform->rotation_euler.x < glm::radians(-89.0f))
	{
		transform->rotation_euler.x = glm::radians(-89.0f);
	}
}
void InputSystem::setup()
{
	quit_flag_ = false;
	keyboard_ = std::make_unique<Keyboard>();
	mouse_ = std::make_unique<Mouse>();
}

void InputSystem::tick()
{
	SDL_Event e = { 0 };
	while (SDL_PollEvent(&e))
	{
		mouse_->update();
		for (auto& controller_instance : mouse_controllers_)
		{
			mouse_->controller(controller_instance.first, controller_instance.second);
		}

		keyboard_->update();

		if (e.type == SDL_QUIT)
		{
			quit_flag_ = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			//locking the cursor on F click
			if (e.key.keysym.sym == SDLK_f)
			{
				if ((bool)SDL_GetRelativeMouseMode())
					SDL_SetRelativeMouseMode(SDL_FALSE);
				else
					SDL_SetRelativeMouseMode(SDL_TRUE);
			}
		}
	}
}

//

//
///**
//*\brief Handles input for mouse orientation.
//*
//* Processes current mouse input and rotates transform as required.
//*/
void InputSystem::attachMouseController(std::shared_ptr<Transform> transform, float sensitivity)
{
	mouse_controllers_.emplace_back(transform, sensitivity);
}
void InputSystem::controlFromKeyboard(std::shared_ptr<Transform> transform, float speed)
{
	if (keyboard_->keyPressed(SDL_SCANCODE_W)) //moving left
	{
		transform->position += speed * transform->forward() * (float)Time::delta_time();
	}
	if (keyboard_->keyPressed(SDL_SCANCODE_S))
	{
		transform->position -= speed * transform->forward() * (float)Time::delta_time();
	}
	if (keyboard_->keyPressed(SDL_SCANCODE_D))
	{
		transform->position += speed * transform->right() * (float)Time::delta_time();
	}
	if (keyboard_->keyPressed(SDL_SCANCODE_A))
	{
		transform->position -= speed * transform->right() * (float)Time::delta_time();
	}
}



