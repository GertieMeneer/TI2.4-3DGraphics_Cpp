#include "PlayerComponent.h"

PlayerComponent::PlayerComponent()
{
	//
}

PlayerComponent::~PlayerComponent()
{

}

void PlayerComponent::update(float deltaTime, Entity &entity, cam &camera) {
	entity.position = camera.getPosition();
}