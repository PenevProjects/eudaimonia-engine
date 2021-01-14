#ifndef _RENDERING_SYSTEM_H
#define _RENDERING_SYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <SDL2/SDL.h>
#include <string>
#include "zero/System.h"

struct Camera;
struct Transform;
class Model;

class RenderingSystem : public zero::IBaseSystem
{
public:
	static int screen_width;
	static int screen_height;
	SDL_Window *window;
	std::string title;

	RenderingSystem() = default;
	void setup();
	void tick();
	void update() {}

	/// Generates and returns a view matrix of the passed camera.
	glm::mat4 viewMatrix(const Transform& transform);
	/// Generates and returns a perspective projection matrix of the passed camera.
	glm::mat4 perspectiveProjection(const Camera& camera);
	/// Generates and returns an ortographic projection matrix of the passed camera.
	glm::mat4 orthoProjection();
};


#endif