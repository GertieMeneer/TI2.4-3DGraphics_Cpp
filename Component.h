#pragma once
#include "Entity.h"
#include "cam.h"

class Component {
public:
	Component();
	~Component();

	virtual void update(float deltaTime, Entity &entity, cam &camera) {};
};
