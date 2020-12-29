#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Component.h"


namespace chrono {

class Transform : public Component {
public:
	Transform();
	~Transform();
	void onInitialize();
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};





} //namespace chrono