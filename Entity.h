#pragma once

#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "ColliderComponent.h"
#include "LifetimeComponent.h"
#include "Component.h"
#include "tigl.h"
#include "cam.h"
#include <memory>
#include <vector>
#include <list>

using tigl::Vertex;

class Entity : public Component {
public:
    glm::vec3 position;

    Entity();
    ~Entity();

    void addComponent(Component* component);
    virtual void update(float deltaTime, Entity &entity, cam &camera) override;
    void draw();

private:
    std::vector<Vertex> vertices;
    std::list<Component*> components;

};
