#pragma once
#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include <glm/vec3.hpp>

struct ColliderComponent {
    glm::vec3 minBounds;
    glm::vec3 maxBounds;
};

#endif // COLLIDER_COMPONENT_H
