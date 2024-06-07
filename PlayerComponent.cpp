#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(cam& camera) : camera(camera)
{

}

PlayerComponent::~PlayerComponent()
{

}

void PlayerComponent::update(float deltaTime) {
	position = camera.getPosition();
}