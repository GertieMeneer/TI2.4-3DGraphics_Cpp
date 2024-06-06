#pragma once

#include <chrono>
#include "Component.h"

class LifetimeComponent : public Component {
public:
    float lifetime;
    std::chrono::steady_clock::time_point spawnTime;

    // Default constructor (compiler-generated)
    LifetimeComponent() = default;

    // Constructor to initialize lifetime and spawnTime
    LifetimeComponent(float lifetime, const std::chrono::steady_clock::time_point& spawnTime);
    ~LifetimeComponent();

    virtual void update(float elapsedTime) override;
};
