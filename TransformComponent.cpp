#include "TransformComponent.h"
// No implementation needed


TransformComponent::TransformComponent(const glm::vec3& position, const glm::vec3& scale)
{
	this->position = position;
	this->scale = scale;
}

TransformComponent::~TransformComponent()
{
	//
}

void TransformComponent::update(float elapsedTime) {

}