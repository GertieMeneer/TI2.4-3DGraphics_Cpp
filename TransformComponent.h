#pragma once

#include <glm/vec3.hpp>
#include "Component.h"

class TransformComponent : public Component {
    glm::vec3 position;
    glm::vec3 scale;

    TransformComponent(const glm::vec3& position, const glm::vec3& scale);
    ~TransformComponent();

    virtual void update(float elapsedTime) override;
};
