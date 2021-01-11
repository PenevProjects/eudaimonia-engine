#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <exception>
#include <vector>
#include <windows.h>





#include "Shader.h"
#include "Camera.h"
#include "Time.h"
#include "Texture.h"
#include "Model.h"
#include "Framebuffer.h"
#include "Skybox.h"

#include "zero/Core.h"
#include "zero/Entity.h"
#include "zero/Component.h"
#include "zero/System.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "RenderingSystem.h"


int main(int argc, char *argv[])
{
	//initial menu
	int skyboxChoice = 1;
	//while (!(skyboxChoice == 1 || skyboxChoice == 2 || skyboxChoice == 3))
	//{
	//	std::cout << "select skybox: [1 | 2 | 3]: ";
	//}
	bool renderCar = false;
	char carSelection;
	std::cout << "render car model?: [y | n]: ";
	std::cin >> carSelection;
	if (carSelection == 'y' || carSelection == 'Y')
	{
		renderCar = true;
	}


	std::unique_ptr<zero::Core> core = zero::Core::createCore();
	auto em = core->entity_manager();
	auto cm = core->component_manager();
	auto sm = core->systems_manager();

	auto movement_s = sm->addSystem<TransformSystem>();
	auto rendering_s = sm->addSystem<RenderingSystem>();

	cm->createComponentType<Transform>();
	cm->createComponentType<Camera>();

	zero::Entity* one = em->createEntity();
	auto one_transform = one->addComponent<Transform>(glm::vec3(66.0f, 66.0f, 66.0f));

	zero::Entity* two = em->createEntity();
	auto two_transform = two->addComponent<Transform>(glm::vec3(87.0f, 89.0f, 82.0f));
	//tv->m_modelMatrix = glm::scale(tv->m_modelMatrix, glm::vec3(0.3f));
	//tv->m_modelMatrix = glm::rotate(tv->m_modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//tv->m_modelMatrix = glm::translate(tv->m_modelMatrix, glm::vec3(-40.0f, 0.0f, -10.0f));

	two->copyComponent<Transform>(one);
	std::cout << std::endl << glm::to_string(one_transform->model_matrix()) << std::endl;;
	one_transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
	one_transform->scale = glm::vec3(0.3f);

	movement_s->update(one_transform);




	auto cam1 = em->createEntity();
	auto cam1_transform = cam1->addComponent<Transform>(glm::vec3{ 0.0f, 0.0f, 15.0 });
	auto cam1_camera = cam1->addComponent<Camera>();

	movement_s->update(cam1_transform);

	glm::mat4 view_mat = rendering_s->viewMatrix(cam1_transform.get());
	glm::mat4 proj_mat = rendering_s->perspectiveProjection(cam1_camera.get());




	std::unique_ptr<Shader> lampShader = std::make_unique<Shader>("../src/shaders/pure-white.vert", "../src/shaders/pure-white.frag");
	std::unique_ptr<Shader> skyboxShader = std::make_unique<Shader>("../src/shaders/skybox.vert", "../src/shaders/skybox.frag");
	std::unique_ptr<Shader> pbrShader = std::make_unique<Shader>("../src/shaders/pbr/pbr.vert", "../src/shaders/pbr/pbr.frag");



	//Max number of texture units that can be used concurrently from a model file is currently 9. 
	unsigned int skyboxSamplerID = 10;

	//skybox - this also constructs maps for irradiance, prefilter and brdfLUT
	std::shared_ptr<Skybox> skybox;
	if (skyboxChoice == 1)
	{
		skybox = std::make_shared<Skybox>("../assets/hdr/night4k.hdr", 2048);
	}
	else if (skyboxChoice == 2)
	{
		skybox = std::make_shared<Skybox>("../assets/hdr/Road_to_MonumentValley_Ref.hdr", 2048);
	}
	else
	{
		skybox = std::make_shared<Skybox>("../assets/hdr/Factory_Catwalk_2k.hdr", 2048);
	}
	//OTHER SKYBOXES
	//std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>("../assets/hdr/Road_to_MonumentValley_Ref.hdr", 2048);
	//std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>("../assets/hdr/Factory_Catwalk_2k.hdr", 2048);


	//car - DONT FORGET DRAW CALL in main game loop.
	std::shared_ptr<Model> car;
	if (renderCar)
	{
		car = std::make_shared<Model>("../assets/car/car.fbx");
		car->m_modelMatrix = glm::translate(car->m_modelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
	}

	//tv pbr
	std::shared_ptr<Model> tv = std::make_shared<Model>("../assets/tv/tv.fbx");
	tv->m_modelMatrix = one_transform->model_matrix();

	//mask pbr
	std::shared_ptr<Model> oniMask = std::make_shared<Model>("../assets/oni/onito.fbx");
	oniMask->m_modelMatrix = glm::scale(oniMask->m_modelMatrix, glm::vec3(1.0f));
	oniMask->m_modelMatrix = glm::translate(oniMask->m_modelMatrix, glm::vec3(15.0f, -5.0f, 0.0f));
	oniMask->m_modelMatrix = one_transform->model_matrix();

	//sword pbr
	std::shared_ptr<Model> sword = std::make_shared<Model>("../assets/sword/sword.fbx");
	sword->m_modelMatrix = glm::scale(sword->m_modelMatrix, glm::vec3(10.0f));
	sword->m_modelMatrix = glm::rotate(sword->m_modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));



	glm::vec3 lightPos[] {
		glm::vec3{ 0.0f, 1.0f, 15.0f},
		glm::vec3{ 25.0f, 3.0f, 10.0f},
		glm::vec3{-25.0f, 3.0f, 10.0f},
	};
	glm::vec3 lightColors[] {
		glm::vec3{300.0f, 300.0f, 300.0f},
		glm::vec3{100.0f, 100.0f, 100.0f},
		glm::vec3{100.0f, 100.0f, 100.0f},
	};

	//lamp 0
	std::shared_ptr<Model> lamp0 = std::make_shared<Model>("../assets/cube.obj");

	//lamp 1 - static
	std::shared_ptr<Model> lamp1 = std::make_shared<Model>("../assets/cube.obj");
	lamp1->m_modelMatrix = glm::translate(glm::mat4{ 1.0f }, lightPos[1]);
	lamp1->m_modelMatrix = glm::scale(lamp1->m_modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

	//lamp 2 - static
	std::shared_ptr<Model> lamp2 = std::make_shared<Model>("../assets/cube.obj");
	lamp2->m_modelMatrix = glm::translate(glm::mat4{ 1.0f }, lightPos[2]);
	lamp2->m_modelMatrix = glm::scale(lamp2->m_modelMatrix, glm::vec3{0.2f, 0.2f, 0.2f});


	//camera
	//std::shared_ptr<Camera> cam1 = std::make_shared<Camera>(glm::vec3{ 0.0f, 0.0f, 15.0 });

	bool quit = false;
	float translation{ 0.0f };


	pbrShader->Use();
	pbrShader->setInt("u_irradianceMap", skyboxSamplerID + 1);
	pbrShader->setInt("u_prefilterMap", skyboxSamplerID + 2);
	pbrShader->setInt("u_brdfLUT", skyboxSamplerID + 3);
	pbrShader->setMat4("u_View", view_mat);
	pbrShader->setMat4("u_Projection", proj_mat);


	for (unsigned int i = 0; i < sizeof(lightPos) / sizeof(lightPos[0]); i++)
	{
		pbrShader->setVec3("u_lightPos[" + std::to_string(i) + "]", lightPos[i]);
		pbrShader->setVec3("u_lightColors[" + std::to_string(i) + "]", lightColors[i]);
	}
	pbrShader->StopUsing();



	//SKYBOX////////////////////////////////////////////////

	skyboxShader->Use();
	skyboxShader->setInt("u_environmentCubemap", skyboxSamplerID);
	skyboxShader->setMat4("u_View", view_mat);
	skyboxShader->setMat4("u_Projection", proj_mat);
	skyboxShader->StopUsing();
	//wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	Time::Update();

	while (!quit)
	{
		SDL_Event e = { 0 };
		while (SDL_PollEvent(&e))
		{

			if (e.type == SDL_QUIT)
			{
				quit = true;
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
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				//locking the cursor on mouse click
				if ((bool)SDL_GetRelativeMouseMode())
					SDL_SetRelativeMouseMode(SDL_FALSE);
				else
					SDL_SetRelativeMouseMode(SDL_TRUE);
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				//process mouse input
				float deltaX = static_cast<float>(e.motion.xrel);
				float deltaY = static_cast<float>(-e.motion.yrel);
				//cam1->ProcessMouseInput(deltaX, deltaY);
			}

		}
		//time calculations
		Time::Update();
		Time::DisplayFPSinWindowTitle(rendering_s->window);



		rendering_s->tick();

		//camera updates
		//cam1->ProcessKeyboardInput();
		//cam1->ProcessZoom();
		//cam1->setWindowSize(width, height);

	

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuf1->GetID());
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		pbrShader->Use();
		pbrShader->setVec3("u_viewPos", cam1_transform->position);
		pbrShader->setVec3("u_lightPos[0]", lightPos[0]);
		pbrShader->setMat4("u_View", view_mat);
		pbrShader->setMat4("u_Projection", proj_mat);

		// bind pre-computed IBL data
		glActiveTexture(GL_TEXTURE0 + skyboxSamplerID + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->GetIrradianceMap().lock()->GetId());
		glActiveTexture(GL_TEXTURE0 + skyboxSamplerID + 2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->GetPrefilterMap().lock()->GetId());
		glActiveTexture(GL_TEXTURE0 + skyboxSamplerID + 3);
		glBindTexture(GL_TEXTURE_2D, skybox->GetBrdfLUT().lock()->GetId());

		//UNCOMMENT FOR CAR
		//if (renderCar)
		//{
		//	car->RenderMeshes(*pbrShader);
		//}

		//tv->RenderMeshes(*pbrShader);
		oniMask->RenderMeshes(*pbrShader, one_transform.get());
		//sword->RenderMeshes(*pbrShader);

		pbrShader->StopUsing();

		//lampShader->Use();
		////main lamp translation
		//float speed = 0.001f;
		//float range = 10.0f;
		//translation = glm::sin(SDL_GetTicks()*speed)*range; //oscillate
		//lightPos[0].x = translation;

		//lampShader->setViewAndProjectionMatrix(*cam1, true);
		//lamp0->m_modelMatrix = glm::translate(glm::mat4{ 1.0f }, lightPos[0]);
		//lamp0->m_modelMatrix = glm::scale(lamp0->m_modelMatrix, glm::vec3{ 0.4f });
		//lamp0->RenderMeshes(*lampShader);
		//lamp1->RenderMeshes(*lampShader);
		//lamp2->RenderMeshes(*lampShader);

		//lampShader->StopUsing();


		///////////////////////SKYBOX//////////////////////
		skyboxShader->Use();
		glActiveTexture(GL_TEXTURE0 + skyboxSamplerID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->GetSkyboxMap().lock()->GetId());
		skybox->RenderCube();
		skyboxShader->StopUsing();
		glBindVertexArray(0);
			
		SDL_GL_SwapWindow(rendering_s->window);
		Time::Reset();
	}

	SDL_DestroyWindow(rendering_s->window);
	SDL_Quit();

	return 0;
}