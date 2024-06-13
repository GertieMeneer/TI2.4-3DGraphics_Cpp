#include "PlayerComponent.h"
#include <iostream>

PlayerComponent::PlayerComponent(cam *camera) : camera(camera)
{

}

PlayerComponent::~PlayerComponent()
{

}

void PlayerComponent::update(float deltaTime) {
	position = camera->getPosition();
}