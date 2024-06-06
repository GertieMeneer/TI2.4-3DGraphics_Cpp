#include "ColliderComponent.h"
// No implementation needed

ColliderComponent::ColliderComponent(glm::vec3 minBounds, glm::vec3 maxBounds)
{
	this->minBounds = minBounds;
	this->maxBounds = maxBounds;
}

ColliderComponent::~ColliderComponent() {}

void ColliderComponent::update(float elapsedTime) {

}