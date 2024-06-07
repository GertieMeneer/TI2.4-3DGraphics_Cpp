#pragma once
#include "tigl.h"
#include "Entity.h"

#include <vector>
#include <iostream>
#include <chrono>


using tigl::Vertex;

class Util
{
public:
	static std::vector<Vertex> buildCube(const glm::vec3& p, const glm::vec3& s, const glm::vec4& color);
	static std::vector<Vertex> buildFloor();
	static void drawPlayerColliderBoundsBox(Entity* player);
	static void drawParticleColliderBoundsBox(const std::vector<std::unique_ptr<Entity>>& entities);
	static void SaveScore(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end, std::string reason);
};
