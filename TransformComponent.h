#pragma once

#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/vec3.hpp>

struct TransformComponent {
    glm::vec3 position;
    glm::vec3 scale;
};

#endif // TRANSFORM_COMPONENT_H
