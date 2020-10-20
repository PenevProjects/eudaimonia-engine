#include "core.h"



namespace eudaimonia
{
	std::shared_ptr<Core> Core::initialize()
	{
		std::shared_ptr<Core> obj = std::make_shared<Core>();
		obj->self = obj;

		obj->window = SDL_CreateWindow("eudaimonia engine",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			800, 600,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		if (!obj->window)
		{
			printf("window failed");
			throw;
		}
		
		return obj;
	}

	std::shared_ptr<Entity> Core::addEntity()
	{
		std::shared_ptr<Entity> entityObj = std::make_shared<Entity>();
		entityObj->core = self;
		entityObj->self = entityObj;

		entities.push_back(entityObj);

		return entityObj;
	}
}