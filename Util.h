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
	static std::vector<Vertex> buildHitbox(glm::vec3 minBounds, glm::vec3 maxBounds);
	static void drawPlayerColliderBoundsBox(Entity* player, cam* camera);
	static void drawParticleColliderBoundsBox(Entity* player, std::vector<std::unique_ptr<Entity>> entities, cam* camera);

};