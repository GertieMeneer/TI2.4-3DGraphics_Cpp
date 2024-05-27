#pragma once
#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H

#include "tigl.h"
#include <vector>

struct RenderableComponent {
    std::vector<tigl::Vertex> vertices;
};

#endif // RENDERABLE_COMPONENT_H
