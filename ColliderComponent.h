#pragma once

#include <glm/vec3.hpp>
#include "Component.h"

class ColliderComponent : public Component {
public:
    glm::vec3 minBounds;
    glm::vec3 maxBounds;

    ColliderComponent(glm::vec3 minBounds, glm::vec3 maxBounds);
    ~ColliderComponent();

    virtual void update(float elapsedTime) override;

};
