#pragma once

#include "Component.h"

#include <chrono>

class LifetimeComponent : public Component {
public:
    float lifetime;
    std::chrono::steady_clock::time_point spawnTime;
    bool toBeRemoved = false;

    LifetimeComponent(float lifetime, const std::chrono::steady_clock::time_point& spawnTime);
    ~LifetimeComponent();

    virtual void update(float deltaTime) override;
};
