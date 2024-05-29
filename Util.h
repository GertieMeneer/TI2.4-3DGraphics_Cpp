#pragma once

#include <vector>
#include <vector>
#include "tigl.h"
#include "Entity.h"
#include "Cam.h"

using tigl::Vertex;

class Util
{
public:
	static std::vector<Vertex> buildCube(const glm::vec3& p, const glm::vec3& s, const glm::vec4& color);
	static std::vector<Vertex> buildFloor();
	static void drawPlayerColliderBoundsBox(Entity* player, cam* camera);
	static void drawParticleColliderBoundsBox(const std::vector<std::unique_ptr<Entity>>& entities, const Entity* player);

};