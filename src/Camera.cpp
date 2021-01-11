#include "Camera.h"
#include "Time.h"

//extern const float defaultYAW = -90.0f;
//extern const float defaultPITCH = 0.0f;
//extern const float defaultSPEED = 10.0f;
//extern const float defaultSENSITIVITY = 0.1f;
//extern const float defaultZOOM = 45.0f;


//int Camera::screen_width_ = WINDOW_WIDTH;
//int Camera::screen_height_ = WINDOW_HEIGHT;
//
//Camera::Camera(glm::vec3 _position, glm::vec3 _facing, glm::vec3 _up) :
//	yaw_(defaultYAW),
//	pitch_(defaultPITCH),
//	m_CameraSensitivity(defaultSENSITIVITY),
//	m_CameraMovementSpeed(defaultSPEED),
//	fov_(defaultZOOM),
//	m_Position(_position),
//	m_Facing(_facing),
//	m_Up(_up),
//	m_WorldUp(glm::vec3(0, 1.0f, 0))
//{
//}
//
//
//
///**
//*\brief Handles input for translation and updates position.
//*
//* Processes current keystroke and transforms the camera's position using current orientation of camera.
//*/
//void Camera::ProcessKeyboardInput()
//{
//	const Uint8* key = SDL_GetKeyboardState(NULL);
//	if (key[SDL_SCANCODE_W]) //moving left
//	{
//		m_Position += m_CameraMovementSpeed * m_Facing * (float)Time::GetDeltaTime();
//	}
//	if (key[SDL_SCANCODE_S])
//	{
//		m_Position -= m_CameraMovementSpeed * m_Facing * (float)Time::GetDeltaTime();
//	}
//	if (key[SDL_SCANCODE_D])
//	{
//		m_Position += glm::normalize(glm::cross(m_Facing, m_Up)) * m_CameraMovementSpeed * (float)Time::GetDeltaTime();
//	}
//	if (key[SDL_SCANCODE_A])
//	{
//		m_Position -= glm::normalize(glm::cross(m_Facing, m_Up)) * m_CameraMovementSpeed * (float)Time::GetDeltaTime();
//	}
//}
//
///**
//*\brief Handles input for mouse orientation.
//*
//* Processes current keystroke and transforms the camera's position using current orientation of camera.
//*/
//void Camera::ProcessMouseInput(float xoffset, float yoffset)
//{
//	yaw_ += (xoffset * m_CameraSensitivity ); //* Time::GetDeltaTime()
//	pitch_ += (yoffset * m_CameraSensitivity ); //* Time::GetDeltaTime()
//
//	if (pitch_ > 89.0f)
//	{
//		pitch_ = 89.0f;
//	}
//	if (pitch_ < -89.0f)
//	{
//		pitch_ = -89.0f;
//	}
//
//	UpdateCameraVectors();
//
//
//
//}
//
///**
//* \brief Updates camera orientation according to mouse input.
//*
//* considering pitch, directions in 3D can be presented as
//* x = cos(yaw) * cos(pitch)
//* z = sin(yaw) * cos(pitch)
//* y = sin(yaw)
//* deduct other orientation vectors from these.
//*/
//void Camera::UpdateCameraVectors()
//{
//	glm::vec3 direction;
//	direction.x = glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
//	direction.z = glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
//	direction.y = glm::sin(glm::radians(pitch_));
//	m_Facing = glm::normalize(direction);
//	m_Right = glm::normalize(glm::cross(m_Facing, m_WorldUp));
//	m_Up = glm::normalize(glm::cross(m_Right, m_Facing));
//}
//
///// In case of window resize, updates width and height for perspective projection calculation.
//void Camera::setWindowSize(unsigned int _screenWidth, unsigned int _screenHeight)
//{
//	screen_width_ = _screenWidth;
//	screen_height_ = _screenHeight;
//}
//
///// Handles keyboard input for zooming with Q and E (works with Field of View).
//void Camera::ProcessZoom()
//{
//	const Uint8* key = SDL_GetKeyboardState(NULL);
//	if (key[SDL_SCANCODE_Q])
//	{
//		fov_ += 1;
//	}
//	if (key[SDL_SCANCODE_E])
//	{
//		fov_ -= 1;
//	}
//
//}
//
//glm::mat4 Camera::generateViewMatrix() const
//{
//	return glm::lookAt(m_Position, m_Position + m_Facing, m_Up);
//}
//
//glm::mat4 Camera::generateProjMatrixPersp() const
//{
//	return glm::perspective(glm::radians(fov_), (float)screen_width_ / (float)screen_height_, 0.1f, 1000.0f);
//}
//glm::mat4 Camera::generateProjMatrixOrtho() const
//{
//	return glm::ortho((float)-screen_width_/2, (float)screen_width_/2, (float)screen_height_/2, (float)-screen_height_/2, 0.1f, 100.0f);
//}