#pragma once

#include "ColliderComponent.h"
#include "LifetimeComponent.h"
#include "PlayerComponent.h"
#include "VelocityComponent.h"
#include "ModelComponent.h"
#include "Texture.h"

#include "tigl.h"
#include <memory>
#include <vector>
#include <list>

using tigl::Vertex;

class Entity {
public:
    glm::vec3 position;
    std::vector<Vertex> vertices;
    Texture* texture;

    bool toBeRemoved = false;
    std::string name;

    ColliderComponent* colliderComponent = nullptr;
    LifetimeComponent* lifetimeComponent = nullptr;
    PlayerComponent* playerComponent = nullptr;
    VelocityComponent* velocityComponent = nullptr;
    ModelComponent* modelComponent = nullptr;

    Entity();
    ~Entity();

    void update(float deltaTime);
    void draw();

};
