#include "Entity.h"
#include <iostream>

Entity::Entity() 
{
	//
}

Entity::~Entity()
{
	//
}

void Entity::update(float deltaTime) {
	if(lifetimeComponent) 
	{
		lifetimeComponent->update(deltaTime);
		if (lifetimeComponent->toBeRemoved) {
			toBeRemoved = true;
		}
	}
	if (playerComponent)
	{
		this->position = this->playerComponent->position;
		playerComponent->update(deltaTime);
	}
	if (velocityComponent)
	{
		this->position += this->velocityComponent->velocity * deltaTime;
		for (auto& vertex : vertices)
		{
			vertex.position += velocityComponent->velocity * deltaTime;
		}
	}
}

void Entity::draw() {
	tigl::drawVertices(GL_QUADS, vertices);
}