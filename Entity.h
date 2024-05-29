#pragma once

#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "RenderableComponent.h"
#include "ColliderComponent.h"
#include "LifetimeComponent.h"
#include <memory>

class Entity {
public:
    TransformComponent transform;
    std::unique_ptr<VelocityComponent> velocity;
    std::unique_ptr<RenderableComponent> renderable;
    std::unique_ptr<ColliderComponent> collider;
    std::unique_ptr<LifetimeComponent> lifetime;

    Entity();
};
