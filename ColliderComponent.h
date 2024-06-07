#pragma once

#include <glm/vec3.hpp>

class ColliderComponent {
public:
    glm::vec3 minBounds;
    glm::vec3 maxBounds;

    ColliderComponent(glm::vec3 minBounds, glm::vec3 maxBounds);
    ~ColliderComponent();

};
