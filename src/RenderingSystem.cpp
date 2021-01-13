#include "RenderingSystem.h"
#include "Transform.h"
#include "Camera.h"
#include "ModelRenderer.h"

int RenderingSystem::screen_width_ = 1280;
int RenderingSystem::screen_height_ = 720;

void RenderingSystem::setup()
{
	// Global SDL state
	// -------------------------------
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return;
	}

	SDL_GL_SetSwapInterval(0); //disable vsync evil
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);


	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		screen_width_ = 800;
		screen_height_ = 600;
	}
	else
	{
		screen_width_ = int(dm.w * 0.8);
		screen_height_ = int(dm.h * 0.8);
	}

	


	window_ = SDL_CreateWindow("ZERO DEMO. FPS: ",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_width_, screen_height_,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Lock in mouse
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	if (!SDL_GL_CreateContext(window_))
	{
		SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
		return;
	}


	// Global OpenGL state
	// ---------------------------------
	if (glewInit() != GLEW_OK)
	{
		std::cout << "glewInit failed!" << std::endl;
		return;
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // for skybox rendering
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); //// for lower mip levels in the pre-filter map.

	glEnable(GL_MULTISAMPLE); // turn on multisample anti-aliasing
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); //ensure multisampling is nicest quality
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); //ensure multisampling is nicest quality
}

void RenderingSystem::tick()
{
	//process window size
	SDL_GetWindowSize(window_, &screen_width_, &screen_height_);
	glViewport(0, 0, screen_width_, screen_height_);
}


glm::mat4 RenderingSystem::viewMatrix(const Transform& transform)
{
	return glm::lookAt(transform.position, {transform.position + transform.forward()}, transform.up());
}
glm::mat4 RenderingSystem::perspectiveProjection(const Camera& camera)
{
	return glm::perspective(glm::radians(camera.fov_), (float)screen_width_ / (float)screen_height_, 0.1f, 1000.0f);
}
glm::mat4 RenderingSystem::orthoProjection()
{
	return glm::ortho(-float(screen_width_ / 2), float(screen_width_ / 2), float(screen_height_ / 2), -float(screen_height_ / 2), 0.1f, 100.0f);
}

