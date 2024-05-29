#pragma once

#include "tigl.h"
#include <vector>

struct RenderableComponent {
    std::vector<tigl::Vertex> vertices;

    // Default constructor (compiler-generated)
    RenderableComponent() = default;
};
