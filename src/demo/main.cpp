#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <exception>
#include <vector>

#include "../zero/zero.h"

#include "../eudaimonia.h"

//DEMO

int main(int argc, char *argv[])
{
	bool pp_flag = false;
	bool camera_model_flag = false;
	char selection;
	std::cout << "> Enable post-processing?: [y | n]: ";
	std::cin >> selection;
	if (selection == 'y' || selection == 'Y')
	{
		pp_flag = true;
	}
	std::cout << "> Load complex model? (est. ~1min) [y | n]: ";
	std::cin >> selection;
	if (selection == 'y' || selection == 'Y')
	{
		camera_model_flag = true;
	}

	// Initialize zero framework
	std::unique_ptr<zero::Core> core = zero::Core::createCore();
	auto em = core->entity_manager();
	auto cm = core->component_manager();
	auto sm = core->systems_manager();

	// Initialize System types
	// They tick in order of initialization
	auto movement_s = sm->createSystem<TransformSystem>();
	auto rendering_s = sm->createSystem<RenderingSystem>();
	auto input_s = sm->createSystem<InputSystem>();
	auto resource_s = sm->createSystem<ResourceSystem>();

	// Initialize Component types
	cm->createComponentType<Transform>();
	cm->createComponentType<Camera>();
	cm->createComponentType<ModelRenderer>();

	auto cam1 = em->createEntity();
	auto cam1_transform = cam1->addComponent<Transform>(glm::vec3{ 0.0f, 0.0f, 10.0 });
	auto cam1_camera = cam1->addComponent<Camera>();
	input_s->attachMouseController(cam1_transform, 0.5f);

	// Graphics Pipeline Setup
	glm::mat4 view_mat = rendering_s->viewMatrix(*cam1_transform);
	glm::mat4 proj_mat = rendering_s->projectionPerspective(*cam1_camera);
	std::shared_ptr<Shader> lampShader = std::make_shared<Shader>("../src/shaders/pure-white.vert", "../src/shaders/pure-white.frag");
	std::shared_ptr<Shader> skyboxShader = std::make_shared<Shader>("../src/shaders/skybox.vert", "../src/shaders/skybox.frag");
	std::shared_ptr<Shader> pbrShader = std::make_shared<Shader>("../src/shaders/pbr/pbr.vert", "../src/shaders/pbr/pbr.frag");
	std::shared_ptr<Shader> geShader = std::make_shared<Shader>("../src/shaders/pbr/framebuf-quad.vert", "../src/shaders/glowing-edges.frag");
	std::shared_ptr<FrameBuffer> ppFramebuf = std::make_shared<FrameBuffer>(rendering_s->screen_width, rendering_s->screen_height);

	// Max number of texture units that can be used concurrently from a model file is currently 9. 
	unsigned int skyboxSamplerID = 10;
	// skybox - this also constructs maps for irradiance, prefilter and brdfLUT
	std::shared_ptr<Skybox> skybox;
	skybox = std::make_shared<Skybox>("../assets/hdr/map.hdr", 4096);

	// Entities setup
	zero::Entity* one = em->createEntity();
	auto one_transform = one->addComponent<Transform>();
	one_transform->position += glm::vec3{ 0.0f, -5.0f, -20.0f };
	one_transform->scale = glm::vec3{ 0.5f };
	one_transform->rotation_euler.x = glm::radians(-90.0f);
	auto sword_model_file = resource_s->load<Model>("../assets/tv/tv.fbx");
	auto one_model = one->addComponent<ModelRenderer>(sword_model_file, one_transform, pbrShader);

	zero::Entity* two = em->createEntity();
	auto two_transform = two->addComponent<Transform>();
	std::shared_ptr<ModelRenderer> two_model;
	if (camera_model_flag)
	{
		two_transform->scale = glm::vec3{ 50.0f };
		two_transform->rotation_euler.x = glm::radians(-90.0f);
		auto car_model_file = resource_s->load<Model>("../assets/cam/cam.fbx");
		auto car_model = two->addComponent<ModelRenderer>(car_model_file, two_transform, pbrShader);
		one_transform->position += glm::vec3(20.0f, 0.0f, 0.0f);
		two_model = two->getComponent<ModelRenderer>();
	}

	// setup light positions
	glm::vec3 lightPos[] {
		glm::vec3{ 0.0f, 1.0f, 12.0f},
		glm::vec3{ 10.0f, 3.0f, 0.0f},
		glm::vec3{-10.0f, 3.0f, 0.0f},
	};
	glm::vec3 lightColors[] {
		glm::vec3{300.0f, 300.0f, 300.0f},
		glm::vec3{100.0f, 100.0f, 100.0f},
		glm::vec3{100.0f, 100.0f, 100.0f},
	};
	// lamp 0
	auto cube = resource_s->load<Model>("../assets/cube/cube.obj");
	// lamp 0 - dynamic, large
	auto lamp0 = em->createEntity();
	auto lamp0_transform = lamp0->addComponent<Transform>(lightPos[0], glm::vec3(0.0f), glm::vec3(0.4f));
	auto lamp0_model = lamp0->addComponent<ModelRenderer>(cube, lamp0_transform, lampShader);
	// lamp 1 - static, small
	auto lamp1 = em->createEntity();
	auto lamp1_transform = lamp1->addComponent<Transform>(lightPos[1], glm::vec3(0.0f), glm::vec3(0.2f));
	auto lamp1_model = lamp1->addComponent<ModelRenderer>(cube, lamp1_transform, lampShader);
	// lamp 2 - static, small
	auto lamp2 = em->createEntity();
	auto lamp2_transform = lamp2->addComponent<Transform>(lightPos[2], glm::vec3(0.0f), glm::vec3(0.2f));
	auto lamp2_model = lamp2->addComponent<ModelRenderer>(cube, lamp2_transform, lampShader);
	float translation{ 0.0f };



	// PBR shader variable setup
	pbrShader->use();
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
	pbrShader->stopUsing();

	// SKYBOX shader variable setup
	skyboxShader->use();
	skyboxShader->setInt("u_environmentCubemap", skyboxSamplerID);
	skyboxShader->stopUsing();

	geShader->use();
	geShader->setInt("u_screenTexture", 0);
	geShader->setFloat("u_screenWidth", (float)rendering_s->screen_width);
	geShader->setFloat("u_screenHeight", (float)rendering_s->screen_height);
	geShader->stopUsing();

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Time::update();


	while (!input_s->quit_flag_)
	{
		// tick systems
		sm->tickAll();
		// use keyboard to control camera1's transform
		input_s->controlFromKeyboard(cam1_transform, 10.0f);
		// time calculations
		Time::update();
		Time::displayFPSinWindowTitle(rendering_s->window, rendering_s->title);

		view_mat = rendering_s->viewMatrix(*cam1_transform);
		proj_mat = rendering_s->projectionPerspective(*cam1_camera);
	
		if (pp_flag)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, ppFramebuf->fbo());
		}
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind dynamic variables to pbr shader
		pbrShader->use();
		pbrShader->setVec3("u_viewPos", cam1_transform->position);
		pbrShader->setVec3("u_lightPos[0]", lightPos[0]);
		pbrShader->setMat4("u_View", view_mat);
		pbrShader->setMat4("u_Projection", proj_mat);

		// bind pre-computed IBL data
		glActiveTexture(GL_TEXTURE0 + skyboxSamplerID + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->irradiance_map().lock()->id());
		glActiveTexture(GL_TEXTURE0 + skyboxSamplerID + 2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->prefilter_map().lock()->id());
		glActiveTexture(GL_TEXTURE0 + skyboxSamplerID + 3);
		glBindTexture(GL_TEXTURE_2D, skybox->brdf_lut().lock()->id());

		one_model->render();
		if (camera_model_flag) 
		{
			two_model->render();
		}

		pbrShader->stopUsing();

		lampShader->use();
		lampShader->setMat4("u_View", view_mat);
		lampShader->setMat4("u_Projection", proj_mat);
		//main lamp translation
		float speed = 0.001f;
		float range = 10.0f;
		translation = glm::sin(SDL_GetTicks()*speed)*range; //oscillate
		lightPos[0].x = translation;
		lamp0_transform->position = lightPos[0];

		lamp0_model->render();
		lamp1_model->render();
		lamp2_model->render();
		lampShader->stopUsing();


		//Skybox
		skyboxShader->use();
		skyboxShader->setMat4("u_View", view_mat);
		skyboxShader->setMat4("u_Projection", proj_mat);
		glActiveTexture(GL_TEXTURE0 + skyboxSamplerID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->environment_map().lock()->id());
		skybox->renderCube();
		skyboxShader->stopUsing();

		// if post-processing enabled
		if (pp_flag)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT);
			geShader->use();
			geShader->setFloat("u_screenWidth", (float)rendering_s->screen_width);
			geShader->setFloat("u_screenHeight", (float)rendering_s->screen_height);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ppFramebuf->texture());
			ppFramebuf->drawQuad();
			geShader->stopUsing();
		}

			
		SDL_GL_SwapWindow(rendering_s->window);
		Time::reset();
	}

	SDL_DestroyWindow(rendering_s->window);
	SDL_Quit();

	return 0;
}