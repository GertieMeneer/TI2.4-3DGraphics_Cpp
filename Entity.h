#pragma once

#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "ColliderComponent.h"
#include "LifetimeComponent.h"
#include "tigl.h"
#include <memory>
#include <vector>

class Entity {
public:
    TransformComponent transform;
    std::unique_ptr<VelocityComponent> velocity;
    std::unique_ptr<ColliderComponent> collider;
    std::unique_ptr<LifetimeComponent> lifetime;
    std::vector<tigl::Vertex> vertices;

    Entity();

    void update();

};
