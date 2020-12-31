#pragma once
#include <memory>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <exception>
#include <vector>


namespace chrono
{
class Entity;
class Component;
class Transform;

class Core
{
	friend class Entity;
	friend class Component;
public:
	std::shared_ptr<Core> Initialize();
	std::shared_ptr<Entity> AddEntity();
private:

	//weak ptr for passing to hierarchical objects without transferring ownership.
	std::weak_ptr<Core> m_self;
	SDL_Window *m_window;

};
}