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

	// Define vertices for a square floor using two triangles to cover the entire area
	// Triangle 1
	verts.push_back(Vertex::PC(glm::vec3(-5, -1, -5), glm::vec4(1, 1, 1, 1))); // Bottom-left
	verts.push_back(Vertex::PC(glm::vec3(5, -1, -5), glm::vec4(1, 1, 1, 1)));  // Bottom-right
	verts.push_back(Vertex::PC(glm::vec3(5, -1, 5), glm::vec4(1, 1, 1, 1)));   // Top-right

	// Triangle 2
	verts.push_back(Vertex::PC(glm::vec3(-5, -1, -5), glm::vec4(1, 1, 1, 1))); // Bottom-left
	verts.push_back(Vertex::PC(glm::vec3(5, -1, 5), glm::vec4(1, 1, 1, 1)));   // Top-right
	verts.push_back(Vertex::PC(glm::vec3(-5, -1, 5), glm::vec4(1, 1, 1, 1)));  // Top-left

	// Additional triangle to cover the remaining part of the square
	// Triangle 3
	verts.push_back(Vertex::PC(glm::vec3(-5, -1, -5), glm::vec4(1, 1, 1, 1))); // Bottom-left
	verts.push_back(Vertex::PC(glm::vec3(5, -1, -5), glm::vec4(1, 1, 1, 1)));  // Bottom-right
	verts.push_back(Vertex::PC(glm::vec3(-5, -1, 5), glm::vec4(1, 1, 1, 1)));  // Top-left

	return verts;
}

std::vector<Vertex> Util::drawPlayerColliderBoundsBox(Entity* player)
{
    glm::vec3 minBounds = player->playerComponent->position - glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 maxBounds = player->playerComponent->position + glm::vec3(0.5f, 0.5f, 0.5f);

    std::vector<Vertex> verts;

    //front
    verts.push_back(Vertex::PC(minBounds, glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1, 0, 0, 1)));

    //back
    verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(maxBounds, glm::vec4(1, 0, 0, 1)));
    verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1, 0, 0, 1)));

    //connection front with back
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

void Util::drawParticleColliderBoundsBox(const std::vector<std::unique_ptr<Entity>>& entities)
{
	for (const auto& entityPtr : entities) {
		const Entity& entity = *entityPtr;
			// Get particle's collider bounds
			glm::vec3 minBounds = entity.colliderComponent->minBounds + entity.position;
			glm::vec3 maxBounds = entity.colliderComponent->maxBounds + entity.position;

			// Calculate the corners of the box relative to the particle's position
			glm::vec3 corners[8] = {
				minBounds,
				glm::vec3(maxBounds.x, minBounds.y, minBounds.z),
				glm::vec3(maxBounds.x, minBounds.y, maxBounds.z),
				glm::vec3(minBounds.x, minBounds.y, maxBounds.z),
				glm::vec3(minBounds.x, maxBounds.y, minBounds.z),
				glm::vec3(maxBounds.x, maxBounds.y, minBounds.z),
				maxBounds,
				glm::vec3(minBounds.x, maxBounds.y, maxBounds.z)
			};

			// Draw the box using GL_LINES
			glBegin(GL_LINES);

			// Draw lines between corners to form the edges of the box
			for (int i = 0; i < 4; ++i) {
				// Bottom face
				glVertex3f(corners[i].x, corners[i].y, corners[i].z);
				glVertex3f(corners[(i + 1) % 4].x, corners[(i + 1) % 4].y, corners[(i + 1) % 4].z);

				// Top face
				glVertex3f(corners[i + 4].x, corners[i + 4].y, corners[i + 4].z);
				glVertex3f(corners[((i + 1) % 4) + 4].x, corners[((i + 1) % 4) + 4].y, corners[((i + 1) % 4) + 4].z);

				// Connections between top and bottom faces
				glVertex3f(corners[i].x, corners[i].y, corners[i].z);
				glVertex3f(corners[i + 4].x, corners[i + 4].y, corners[i + 4].z);

			glEnd();
		}
	}
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
