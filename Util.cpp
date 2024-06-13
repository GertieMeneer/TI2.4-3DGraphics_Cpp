#include "Util.h"

std::vector<Vertex> Util::buildCube(const glm::vec3& p, const glm::vec3& s)
{
	std::vector<Vertex> verts;
	//bottom
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, -s.y, -s.z), glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, -s.y, -s.z), glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, -s.y, s.z), glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, -s.y, s.z), glm::vec2(0, 1), glm::vec3(0, -1, 0)));

	//top
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, s.y, -s.z), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, s.y, -s.z), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, s.y, s.z), glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, s.y, s.z), glm::vec2(0, 1), glm::vec3(0, 1, 0)));

	//left
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, -s.y, -s.z), glm::vec2(0, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, s.y, -s.z), glm::vec2(1, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, s.y, s.z), glm::vec2(1, 1), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, -s.y, s.z), glm::vec2(0, 1), glm::vec3(1, 0, 0)));

	//right
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, -s.y, -s.z), glm::vec2(0, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, s.y, -s.z), glm::vec2(1, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, s.y, s.z), glm::vec2(1, 1), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, -s.y, s.z), glm::vec2(0, 1), glm::vec3(-1, 0, 0)));

	//back
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, -s.y, -s.z), glm::vec2(0, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, s.y, -s.z), glm::vec2(1, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, s.y, -s.z), glm::vec2(1, 1), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, -s.y, -s.z), glm::vec2(0, 1), glm::vec3(0, 0, -1)));

	//front
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, -s.y, s.z), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PTN(p + glm::vec3(-s.x, s.y, s.z), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, s.y, s.z), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PTN(p + glm::vec3(s.x, -s.y, s.z), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
	return verts;
}

std::vector<Vertex> Util::buildFloor() {
	std::vector<Vertex> verts;

	verts.push_back(Vertex::PC(glm::vec3(-5, -1, -5), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(5, -1, -5), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(5, -1, 5), glm::vec4(1, 1, 1, 1)));

	verts.push_back(Vertex::PC(glm::vec3(-5, -1, -5), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(5, -1, 5), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(-5, -1, 5), glm::vec4(1, 1, 1, 1)));

	verts.push_back(Vertex::PC(glm::vec3(-5, -1, -5), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(5, -1, -5), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(-5, -1, 5), glm::vec4(1, 1, 1, 1)));

	return verts;
}

std::vector<Vertex> Util::drawPlayerColliderBoundsBox(Entity* player)
{
    glm::vec3 minBounds = player->playerComponent->position - glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 maxBounds = player->playerComponent->position + glm::vec3(0.5f, 0.5f, 0.5f);

    std::vector<Vertex> verts;

    verts.push_back(Vertex::PC(minBounds, glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));

    verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(maxBounds, glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));

    verts.push_back(Vertex::PC(minBounds, glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(maxBounds, glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));

    return verts;
}

std::vector<Vertex> Util::drawParticleColliderBoundsBox(Entity* particle)
{
	glm::vec3 minBounds = particle->position - glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 maxBounds = particle->position + glm::vec3(0.5f, 0.5f, 0.5f);

	std::vector<Vertex> verts;

	verts.push_back(Vertex::PC(minBounds, glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));

	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(maxBounds, glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));

	verts.push_back(Vertex::PC(minBounds, glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(maxBounds, glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));

	return verts;
}

std::vector<Vertex> Util::drawCrosshair(float centerX, float centerY)
{
	float size = 20.0f;

	std::vector<Vertex> verts;

	verts.push_back(Vertex::PC(glm::vec3(centerX - size, centerY, 0), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(centerX + size, centerY, 0), glm::vec4(1, 1, 1, 1)));

	verts.push_back(Vertex::PC(glm::vec3(centerX, centerY - size, 0), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(centerX, centerY + size, 0), glm::vec4(1, 1, 1, 1)));

	return verts;
}
