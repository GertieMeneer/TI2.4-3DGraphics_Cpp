#include "LifetimeComponent.h"

LifetimeComponent::LifetimeComponent(float lifetime, const std::chrono::steady_clock::time_point& spawnTime) 
{
	this->lifetime = lifetime;
	this->spawnTime = spawnTime;
}

LifetimeComponent::~LifetimeComponent() {}

void LifetimeComponent::update(float deltaTime) {
	auto currentTime = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - spawnTime);
	auto timeAlive = duration.count();
	if (timeAlive >= lifetime) {
		toBeRemoved = true;
	}
}