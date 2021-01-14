#include "Skybox.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Shader.h"

#include <iostream>


Skybox::Skybox(std::string _path, unsigned int _size) :
	path_{ _path },
	size_{ _size },
	capture_projection_{ glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f) },
	capture_view_{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)) }
{
	//setup skybox vao
	createCubeVAO();	

	// setup member vars
	// ---------------------------------
	environment_map_ = std::make_shared<Texture>(size_, size_, "cubemap");

	//for the current demo, this is a good trade-off between performance and visuals.
	unsigned int reflectionSize = 256;
	prefilter_map_ = std::make_shared<Texture>(reflectionSize, reflectionSize, "cubemapPrefilter");


	//setup vars
	std::unique_ptr<Shader> equirectToCubemapShader = std::make_unique<Shader>("../src/shaders/pbr/cubemap.vert", "../src/shaders/pbr/equirectangular-to-cubemap.frag");
	std::unique_ptr<Shader> irradianceConvShader = std::make_unique<Shader>("../src/shaders/pbr/cubemap.vert", "../src/shaders/pbr/cubemap-conv.frag");
	std::unique_ptr<Shader> prefilterConvShader = std::make_unique<Shader>("../src/shaders/pbr/cubemap.vert", "../src/shaders/pbr/prefilter-conv.frag");
	std::unique_ptr<Shader> brdfLutShader = std::make_unique<Shader>("../src/shaders/pbr/framebuf-quad.vert", "../src/shaders/pbr/brdf-lut.frag");
	std::unique_ptr<FrameBuffer> environmentFramebuffer = std::make_unique<FrameBuffer>(size_, size_);
	std::shared_ptr<Texture> hdrTexture = std::make_shared<Texture>(path_, "HDR");

	// set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------

	//set static shader uniform variables
	equirectToCubemapShader->use();
	equirectToCubemapShader->setInt("equirectangularMap", 0);
	equirectToCubemapShader->setMat4("u_Projection", capture_projection_);

	//convert HDR equirectangular map to cubemap
	// ----------------------------------------------------------------------
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture->id_);

	// configure the viewport to the capture dimensions.
	glViewport(0, 0, size_, size_);

	glBindFramebuffer(GL_FRAMEBUFFER, environmentFramebuffer->fbo());
	for (unsigned int i = 0; i < 6; ++i)
	{ //render all 6 faces of cube going through the view matrices and store in member texture id
		equirectToCubemapShader->setMat4("u_View", capture_view_[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, environment_map_->id_, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//generate mipmap levels for base environment map after it has been generated for prefilter convolution
	glBindTexture(GL_TEXTURE_CUBE_MAP, environment_map_->id_);
	//enable trilinear filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);


	// create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
	// ----------------------------------------------------------------------
	unsigned int irradianceSize = 32;
	irradiance_map_ = std::make_shared<Texture>(irradianceSize, irradianceSize, "cubemap");
	glBindFramebuffer(GL_FRAMEBUFFER, environmentFramebuffer->fbo());
	glBindRenderbuffer(GL_RENDERBUFFER, environmentFramebuffer->rbo());
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, irradianceSize, irradianceSize);

	// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
	// -----------------------------------------------------------------------------
	irradianceConvShader->use();
	irradianceConvShader->setInt("u_environmentCubemap", 0);
	irradianceConvShader->setMat4("u_Projection", capture_projection_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, environment_map_->id_);
	glViewport(0, 0, irradianceSize, irradianceSize); // configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, environmentFramebuffer->fbo());
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceConvShader->setMat4("u_View", capture_view_[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map_->id_, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
	// ----------------------------------------------------------------------------------------------------

	prefilterConvShader->use();
	prefilterConvShader->setInt("u_environmentCubemap", 0);
	prefilterConvShader->setMat4("u_Projection", capture_projection_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, environment_map_->id_);
	//turn on trilinear filtering for fixing of dots
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, environmentFramebuffer->fbo());
	//256 * pow(0.5, 5) = 8x8 is the smallest mipmap we can sample
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; mip++)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = unsigned int(reflectionSize * std::pow(0.5, mip));
		unsigned int mipHeight = unsigned int(reflectionSize * std::pow(0.5, mip));
		glBindRenderbuffer(GL_RENDERBUFFER, environmentFramebuffer->rbo());
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterConvShader->setFloat("u_roughness", roughness);
		prefilterConvShader->setFloat("u_resolution", (float)reflectionSize);
		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterConvShader->setMat4("u_View", capture_view_[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_map_->id_, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//brdf look-up texture setup
	// --------------------------
	brdf_lut_ = std::make_shared<Texture>(size_, size_, "brdfLUT");
	glBindTexture(GL_TEXTURE_2D, brdf_lut_->id_);
	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, environmentFramebuffer->fbo());
	glBindRenderbuffer(GL_RENDERBUFFER, environmentFramebuffer->rbo());
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size_, size_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdf_lut_->id_, 0);

	glViewport(0, 0, size_, size_);
	brdfLutShader->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	environmentFramebuffer->drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &vao_);
}

void Skybox::createCubeVAO()
{
	float cubeVertexData[] = {
		// positions, don't need texture coordinates because cube is always at origin
		// which makes position coordinates also direction vectors(which can be used to sample a cubemap texture)         
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int cubeVBO;
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(vao_);

	//setup quad vao
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), &cubeVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	glDeleteBuffers(1, &cubeVBO);
}

void Skybox::renderCube()
{
	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}



