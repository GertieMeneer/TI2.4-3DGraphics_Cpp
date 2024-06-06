#include "Entity.h"



Entity::Entity() :

    transform(TransformComponent()),
    velocity(nullptr),
    collider(nullptr),
    lifetime(nullptr) {}

void Entity::update() {

}