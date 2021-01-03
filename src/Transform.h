#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Component.h"
#include <memory>


namespace chrono 
{

class Transform : public Component {
public:
	Transform() 
	{ 
		id = Component::Transform; 
	}
	void OnInitialize();
	glm::mat4 GetModelMatrix() const;

	void setPosition(glm::vec3 _newPosition);
	void setOrientation(glm::vec3 _newOrientation);
	void setScale(glm::vec3 _newScale);

	void Translate(glm::vec3 _Position);
	//void Rotate(glm::vec3 _eulerAngles);
	//void Scale(glm::vec3 _Scale);

private:
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 facing;
	glm::vec3 scale;
};

} //namespace chrono