#pragma once

#include <glm/vec3.hpp>

struct VelocityComponent {
    glm::vec3 velocity;

    // Default constructor (compiler-generated)
    VelocityComponent() = default;

    // Constructor to initialize velocity
    explicit VelocityComponent(const glm::vec3& velocity_)
        : velocity(velocity_) {}
};
