#pragma once

#include <vector>
#include <iostream>
#include <chrono>

#include "tigl.h"
#include "Entity.h"

using tigl::Vertex;

class Util
{
public:
	static std::vector<Vertex> buildCube(const glm::vec3& p, const glm::vec3& s);
	static std::vector<Vertex> buildFloor();
	static std::vector<Vertex> drawPlayerColliderBoundsBox(Entity* player);
	static std::vector<Vertex> drawParticleColliderBoundsBox(Entity* particle);
	static std::vector<Vertex> drawCrosshair(float centerX, float centerY);
};
