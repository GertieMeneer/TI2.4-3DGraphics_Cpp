#include "LifetimeComponent.h"
// No implementation needed


LifetimeComponent::LifetimeComponent(float lifetime, const std::chrono::steady_clock::time_point& spawnTime) 
{
	this->lifetime = lifetime;
	this->spawnTime = spawnTime;
}

LifetimeComponent::~LifetimeComponent() {}

void LifetimeComponent::update(float elapsedTime) {

}