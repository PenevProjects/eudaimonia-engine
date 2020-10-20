#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <exception>
#include <vector>



#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GLuint loadTexture(char const * path);

namespace eudaimonia
{
	class Core;

	//ADD ENTITY OBJ
	int main(int argc, char *argv[])
	{
		
		
		

		//if (SDL_Init(SDL_INIT_VIDEO) < 0)
		//{
		//	throw std::exception();
		//}


		//SDL_Window *window = SDL_CreateWindow("OpenGL labs",
		//	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		//	WINDOW_WIDTH, WINDOW_HEIGHT,
		//	SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		//
		//
		//SDL_SetRelativeMouseMode(SDL_TRUE);

		//if (!SDL_GL_CreateContext(window))
		//{
		//	throw std::exception();
		//}
		//if (glewInit() != GLEW_OK)
		//{
		//	throw std::exception();
		//}

		//std::unique_ptr<Shader> stdShader = std::make_unique<Shader>("../src/standardShader.vert", "../src/standardShader.frag");
		//std::unique_ptr<Shader> basicColorShader = std::make_unique<Shader>("../src/basicColor.vert", "../src/basicColor.frag");
		//std::unique_ptr<Shader> lampShader = std::make_unique<Shader>("../src/lightShader.vert", "../src/lightShader.frag");

		//std::shared_ptr<Entity> curuthers = std::make_shared<Entity>("../src/curuthers.obj");
		//curuthers->model = glm::translate(curuthers->model, glm::vec3(0, -5, -3));


		//std::shared_ptr<Entity> lamp = std::make_shared<Entity>();
		//std::shared_ptr<Entity> cube = std::make_shared<Entity>("../src/cube.obj");

		//cube->model = glm::translate(cube->model, glm::vec3(1, 1, -3));

		//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
		//lamp->model = glm::translate(lamp->model, lightPos);
		//lamp->model = glm::scale(lamp->model, glm::vec3(0.2f));

		//std::shared_ptr<Camera> cam1 = std::make_shared<Camera>();


		//GLuint diffuseMap = loadTexture("../src/Whiskers_diffuse.png");


		////TIME
		//std::unique_ptr<Time> time = std::make_unique<Time>();




		//bool quit = false;
		//float rotation = 0.0f;
		//bool enableCull = true;
		//bool enableBlend = true;

		//double lastTime = 0.0;

		//while (!quit)
		//{
		//	SDL_Event e = { 0 };
		//	while (SDL_PollEvent(&e))
		//	{

		//		if (e.type == SDL_QUIT)
		//		{
		//			quit = true;
		//		}
		//		else if (e.type == SDL_KEYDOWN)
		//		{
		//			if (e.key.keysym.sym == SDLK_b)
		//			{
		//				enableBlend = !enableBlend;
		//			}
		//			else if (e.key.keysym.sym == SDLK_c)
		//			{
		//				enableCull = !enableCull;
		//			}
		//			else if (e.key.keysym.sym == SDLK_f)
		//			{
		//				//locking the cursor
		//				if ((bool)SDL_GetRelativeMouseMode())
		//					SDL_SetRelativeMouseMode(SDL_FALSE);
		//				else
		//					SDL_SetRelativeMouseMode(SDL_TRUE);
		//			}
		//		}
		//		else if (e.type == SDL_MOUSEMOTION)
		//		{
		//			float deltaX = e.motion.xrel;
		//			float deltaY = -e.motion.yrel;
		//			cam1->ProcessMouseMovement(deltaX, deltaY);
		//		}

		//	}
		//	//time calculations
		//	time->updateTime(SDL_GetTicks());
		//	time->DisplayFPSinWindowTitle(window);

		//	//window resizing calculationg
		//	int width = 0;
		//	int height = 0;
		//	SDL_GetWindowSize(window, &width, &height);
		//	glViewport(0, 0, width, height);

		//	//camera updates
		//	cam1->ProcessTranslation();
		//	cam1->ProcessZoom();
		//	cam1->ProcessWindowResizing(width, height);

		//	rotation = 90.0f * time->getDeltaTime();
		//	curuthers->model = glm::rotate(curuthers->model, glm::radians(rotation), glm::vec3(0, 1, 0));


		//	stdShader->Use();
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, diffuseMap);
		//	//stdShader->setInt("u_Texture", 0);
		//	//glActiveTexture(GL_TEXTURE0);
		//	//glBindTexture(GL_TEXTURE_2D, texture);

		//	glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	stdShader->Use();
		//	glEnable(GL_DEPTH_TEST);
		//	//set light position
		//	stdShader->setVec3("lightPosition", lightPos);
		//	//set view position
		//	stdShader->setVec3("viewPosition", cam1->getPosition());
		//	//set light color
		//	stdShader->setVec3("lightColor", glm::vec3(1.0f));
		//	stdShader->UseCamera(cam1, GL_TRUE);
		//	stdShader->RenderObject(curuthers);

		//	if (enableCull)
		//	{
		//		glEnable(GL_CULL_FACE);
		//	}

		//	if (enableBlend)
		//	{
		//		glEnable(GL_BLEND);
		//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//	}

		//	stdShader->RenderObject(curuthers);

		//	glDisable(GL_CULL_FACE);
		//	stdShader->RenderObject(cube);

		//	stdShader->StopUsing();

		//	lampShader->Use();
		//	lampShader->UseCamera(cam1, GL_TRUE);
		//	lampShader->RenderObject(lamp);
		//	lampShader->StopUsing();


		//	glDisable(GL_BLEND);
		//	glDisable(GL_DEPTH_TEST);

		//	glBindVertexArray(0);
		//	glUseProgram(0);

		//	SDL_GL_SwapWindow(window);
		//}

		//glDeleteTextures(1, &diffuseMap);

		//SDL_DestroyWindow(window);
		//SDL_Quit();

		return 0;
	}
}


GLuint loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}