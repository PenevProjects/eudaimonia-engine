#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>

#include "zero/Component.h"




/**
* \brief Camera class which handles camera movement and generates view and projection matrices
*
* Stores values for position and orientation of camera.
* Speed and sensitivity of camera are also configurable as public properties(unnecessary to write getters and setters - safe variables).
*/
struct Camera : public zero::IBaseComponent
{
public:
	float fov_;

public:
	//float m_CameraMovementSpeed;
	//float m_CameraSensitivity;
	///Creates Camera object with default values
	Camera() = default;
	void setup(float fov = 45.0f) { this->fov_ = fov; }
	///Creates Camera object with set position, facing direction, and up direction.
	//void ProcessKeyboardInput();
	//void ProcessMouseInput(float xoffset, float yoffset);
	//static void setWindowSize(unsigned int _screenWidth, unsigned int _screenHeight);
	//void ProcessZoom();
	//void UpdateCameraVectors();
	/// Generates and returns a view matrix of the current camera.
	//glm::mat4 generateViewMatrix() const;
	///// Generates and returns a perspective projection matrix of the current camera.
	//glm::mat4 generateProjMatrixPersp() const;
	///// Generates and returns an ortographic projection matrix of the current camera.
	//glm::mat4 generateProjMatrixOrtho() const;


};
