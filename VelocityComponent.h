#pragma once

#include <glm/vec3.hpp>
#include "Component.h"

class VelocityComponent : public Component {
    glm::vec3 velocity;

    // Default constructor (compiler-generated)
    VelocityComponent() = default;

    // Constructor to initialize velocity
    explicit VelocityComponent(const glm::vec3& velocity_)
        : velocity(velocity_) {}

    virtual void update(float elapsedTime) override;
};
