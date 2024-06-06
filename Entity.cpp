#include "Entity.h"

Entity::Entity() 
{
	//
}

Entity::~Entity()
{
	//
}

void Entity::addComponent(Component* component) {
	components.push_back(component);
}

void Entity::update(float deltaTime, Entity &entity, cam &camera) {
	for (auto &c : components) {
		c->update(deltaTime, entity, camera)
	}
}

void Entity::draw() {
	tigl::drawVertices(GL_QUADS, vertices);
}