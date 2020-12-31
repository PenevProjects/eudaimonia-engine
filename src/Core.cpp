#include "Core.h"
#include "Camera.h"
#include "Entity.h"
#include "Transform.h"

namespace chrono
{



std::shared_ptr<Core> Core::Initialize()
{
	std::shared_ptr<Core> obj = std::make_shared<Core>();
	obj->m_self = obj;

	// Global SDL state
	// -------------------------------
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	SDL_GL_SetSwapInterval(0); //disable vsync evil
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	m_window = SDL_CreateWindow("CHRONO ENGINE: ",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Lock in mouse
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	if (!SDL_GL_CreateContext(m_window))
	{
		throw std::exception();
	}


	// Global OpenGL state
	// ---------------------------------
	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // for skybox rendering
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); //// for lower mip levels in the pre-filter map.

	// turn on multisample anti-aliasing
	glEnable(GL_MULTISAMPLE);
	//ensure multisampling is nicest quality
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	return obj;
}

std::shared_ptr<Entity> Core::AddEntity()
{
	std::shared_ptr<Entity> entityObj = std::make_shared<Entity>();
	entityObj->m_self = entityObj;
	entityObj->m_core = m_self;

	entityObj->transform = entityObj->AddComponent<Transform>();

	return entityObj;
}





} //namespace chrono

