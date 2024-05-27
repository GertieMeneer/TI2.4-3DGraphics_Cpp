#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "RenderableComponent.h"
#include "ColliderComponent.h"
#include "LifetimeComponent.h"
#include <memory>

class Entity {
public:
    std::unique_ptr<TransformComponent> transform;
    std::unique_ptr<VelocityComponent> velocity;
    std::unique_ptr<RenderableComponent> renderable;
    std::unique_ptr<ColliderComponent> collider;
    std::unique_ptr<LifetimeComponent> lifetime;

    Entity();
};

#endif // ENTITY_H
