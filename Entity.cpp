#include "Entity.h"

Entity::Entity() :

    transform(std::make_unique<TransformComponent>()),
    velocity(nullptr),
    renderable(nullptr),
    collider(nullptr),
    lifetime(nullptr) {}
