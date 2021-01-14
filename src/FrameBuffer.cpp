#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "FrameBuffer.h"
#include "Shader.h"
#include <iostream>


FrameBuffer::FrameBuffer(int _width, int _height) :
	width_(_width),
	height_(_height)
{
	glGenFramebuffers(1, &fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	createRenderQuad();
	createRenderTexture();
	createRenderBuffer();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

FrameBuffer::~FrameBuffer()
{
	if (texture_id_) { glDeleteTextures(1, &texture_id_); }
	if (m_screenQuadVAO) { glDeleteVertexArrays(1, &m_screenQuadVAO); }
	if (rbo_) { glDeleteRenderbuffers(1, &rbo_); }
	if (fbo_) { glDeleteFramebuffers(1, &fbo_); }
}

void FrameBuffer::createRenderTexture()
{
	glGenTextures(1, &texture_id_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id_, 0);
}
void FrameBuffer::createRenderBuffer()
{
	glGenRenderbuffers(1, &rbo_);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width_, height_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);
}

void FrameBuffer::createRenderQuad()
{
	//screen quad vertices
	float quadVertexData[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	//// screen quad VAO
	unsigned int quadVBO;
	glGenVertexArrays(1, &m_screenQuadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(m_screenQuadVAO);

	//setup quad vao
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), &quadVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
	glDeleteBuffers(1, &quadVBO);
}

void FrameBuffer::drawQuad()
{
	glBindVertexArray(m_screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}