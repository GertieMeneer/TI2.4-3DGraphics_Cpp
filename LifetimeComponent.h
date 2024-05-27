#pragma once
#ifndef LIFETIME_COMPONENT_H
#define LIFETIME_COMPONENT_H

#include <chrono>

struct LifetimeComponent {
    float lifetime;
    std::chrono::steady_clock::time_point spawnTime;
};

#endif // LIFETIME_COMPONENT_H
