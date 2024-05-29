#pragma once

#include <glm/vec3.hpp>

struct TransformComponent {
    glm::vec3 position;
    glm::vec3 scale;

    // Default constructor (compiler-generated)
    TransformComponent() = default;

    // Constructor to initialize position and scale
    TransformComponent(const glm::vec3& position_, const glm::vec3& scale_)
        : position(position_), scale(scale_) {}
};
