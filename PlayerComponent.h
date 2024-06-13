#pragma once

#include "Component.h"
#include "cam.h"

class PlayerComponent : public Component {
public:
	cam *camera;
	glm::vec3 position;

	PlayerComponent(cam *camera);
	~PlayerComponent();

	virtual void update(float deltaTime) override;
};