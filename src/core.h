#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <iostream>


namespace eudaimonia
{
	class Entity;
class Core
{
	std::shared_ptr<Core> initialize();
	std::shared_ptr<Entity> addEntity();
	void start();
private:
	std::vector<std::shared_ptr<Entity>> entities;
	std::weak_ptr<Core> self;
	SDL_Window* window;
	SDL_GLContext glContenxt;
};
}