#include "ColliderComponent.h"


ColliderComponent::ColliderComponent(glm::vec3 minBounds, glm::vec3 maxBounds)
{
	this->minBounds = minBounds;
	this->maxBounds = maxBounds;
}

ColliderComponent::~ColliderComponent() {}