

//
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