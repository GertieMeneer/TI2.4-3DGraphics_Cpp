#pragma once

#include "glm/vec3.hpp"

class VelocityComponent {
public:
	glm::vec3 velocity;

	VelocityComponent(glm::vec3 velocity);
	~VelocityComponent();
};
