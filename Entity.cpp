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
		playerComponent->update(deltaTime);
	}
	if (velocityComponent)
	{
		for (auto& vertex : vertices)
		{
			vertex.position += velocityComponent->velocity * deltaTime;
		}
	}
}

void Entity::draw() {
	tigl::drawVertices(GL_QUADS, vertices);
}