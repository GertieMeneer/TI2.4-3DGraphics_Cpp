#pragma once

#include "Component.h"
#include "Entity.h"

class PlayerComponent : public Component {
public:
	PlayerComponent();
	~PlayerComponent();

	virtual void update(float deltaTime, Entity &entity, cam &camera) override;
};