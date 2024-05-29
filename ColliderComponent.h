#pragma once

#include <glm/vec3.hpp>

struct ColliderComponent {
    glm::vec3 minBounds;
    glm::vec3 maxBounds;

    // Default constructor (compiler-generated)
    ColliderComponent() = default;

    // Constructor to initialize min and max bounds
    ColliderComponent(const glm::vec3& minBounds_, const glm::vec3& maxBounds_)
        : minBounds(minBounds_), maxBounds(maxBounds_) {}
};
