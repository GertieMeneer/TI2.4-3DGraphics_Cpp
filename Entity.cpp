#include "Entity.h"

Entity::Entity() :

    transform(TransformComponent()),
    velocity(nullptr),
    renderable(nullptr),
    collider(nullptr),
    lifetime(nullptr) {}
