#include <cmath>

#include "Util.h"
#include "Entity.h"
#include "Cam.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

std::vector<Vertex> Util::buildCube(const glm::vec3& p, const glm::vec3& s, const glm::vec4& color)
{
	std::vector<Vertex> verts;
	//bottom
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, -s.y, -s.z), color, glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, -s.y, -s.z), color, glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, -s.y, s.z), color, glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, -s.y, s.z), color, glm::vec2(0, 1), glm::vec3(0, -1, 0)));

	//top
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, s.y, -s.z), color, glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, s.y, -s.z), color, glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, s.y, s.z), color, glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, s.y, s.z), color, glm::vec2(0, 1), glm::vec3(0, 1, 0)));

	//left
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, -s.y, -s.z), color, glm::vec2(0, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, s.y, -s.z), color, glm::vec2(1, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, s.y, s.z), color, glm::vec2(1, 1), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, -s.y, s.z), color, glm::vec2(0, 1), glm::vec3(1, 0, 0)));

	//right
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, -s.y, -s.z), color, glm::vec2(0, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, s.y, -s.z), color, glm::vec2(1, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, s.y, s.z), color, glm::vec2(1, 1), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, -s.y, s.z), color, glm::vec2(0, 1), glm::vec3(-1, 0, 0)));

	//back
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, -s.y, -s.z), color, glm::vec2(0, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, s.y, -s.z), color, glm::vec2(1, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, s.y, -s.z), color, glm::vec2(1, 1), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, -s.y, -s.z), color, glm::vec2(0, 1), glm::vec3(0, 0, -1)));

	//front
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, -s.y, s.z), color, glm::vec2(0, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-s.x, s.y, s.z), color, glm::vec2(1, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, s.y, s.z), color, glm::vec2(1, 1), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(s.x, -s.y, s.z), color, glm::vec2(0, 1), glm::vec3(0, 0, 1)));
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

std::vector<Vertex> Util::buildHitbox(glm::vec3 minBounds, glm::vec3 maxBounds) {
	std::vector<Vertex> verts;

	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// Top face
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// Connect top and bottom faces
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	return verts;
}

void Util::drawPlayerColliderBoundsBox(Entity* player, cam* camera)
{
	// Get player's collider bounds
	glm::vec3 minBounds = player->collider->minBounds;
	glm::vec3 maxBounds = player->collider->maxBounds;

	// Calculate the corners of the box relative to the player's position
	glm::vec3 corners[8] = {
		player->transform.position + glm::vec3(minBounds.x, minBounds.y, minBounds.z),
		player->transform.position + glm::vec3(maxBounds.x, minBounds.y, minBounds.z),
		player->transform.position + glm::vec3(maxBounds.x, minBounds.y, maxBounds.z),
		player->transform.position + glm::vec3(minBounds.x, minBounds.y, maxBounds.z),
		player->transform.position + glm::vec3(minBounds.x, maxBounds.y, minBounds.z),
		player->transform.position + glm::vec3(maxBounds.x, maxBounds.y, minBounds.z),
		player->transform.position + glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z),
		player->transform.position + glm::vec3(minBounds.x, maxBounds.y, maxBounds.z)
	};

	// Apply camera's view matrix
	glm::mat4 viewMatrix = camera->getMatrix();

	// Draw the box using GL_LINES
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f); // Set color to white

	// Draw lines between corners to form the edges of the box
	for (int i = 0; i < 4; ++i)
	{
		// Bottom face
		glVertex3f(corners[i].x, corners[i].y, corners[i].z);
		glVertex3f(corners[(i + 1) % 4].x, corners[(i + 1) % 4].y, corners[(i + 1) % 4].z);

		// Top face
		glVertex3f(corners[i + 4].x, corners[i + 4].y, corners[i + 4].z);
		glVertex3f(corners[((i + 1) % 4) + 4].x, corners[((i + 1) % 4) + 4].y, corners[((i + 1) % 4) + 4].z);

		// Connections between top and bottom faces
		glVertex3f(corners[i].x, corners[i].y, corners[i].z);
		glVertex3f(corners[i + 4].x, corners[i + 4].y, corners[i + 4].z);
	}

	glEnd();
}

void Util::drawParticleColliderBoundsBox(Entity* player, std::vector<std::unique_ptr<Entity>> entities, cam* camera)
{
	// Loop through all entities and draw collider boxes for particles
	for (const auto& entity : entities)
	{
		// Check if the entity is a particle and has a collider
		if (entity.get() != player && entity->collider)
		{
			// Get particle's collider bounds
			glm::vec3 minBounds = entity->collider->minBounds;
			glm::vec3 maxBounds = entity->collider->maxBounds;

			// Calculate the corners of the box relative to the particle's position
			glm::vec3 corners[8] = {
				entity->transform.position + glm::vec3(minBounds.x, minBounds.y, minBounds.z),
				entity->transform.position + glm::vec3(maxBounds.x, minBounds.y, minBounds.z),
				entity->transform.position + glm::vec3(maxBounds.x, minBounds.y, maxBounds.z),
				entity->transform.position + glm::vec3(minBounds.x, minBounds.y, maxBounds.z),
				entity->transform.position + glm::vec3(minBounds.x, maxBounds.y, minBounds.z),
				entity->transform.position + glm::vec3(maxBounds.x, maxBounds.y, minBounds.z),
				entity->transform.position + glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z),
				entity->transform.position + glm::vec3(minBounds.x, maxBounds.y, maxBounds.z)
			};

			// Apply camera's view matrix
			glm::mat4 viewMatrix = camera->getMatrix();

			// Draw the box using GL_LINES
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f); // Set color to white

			// Draw lines between corners to form the edges of the box
			for (int i = 0; i < 4; ++i)
			{
				// Bottom face
				glVertex3f(corners[i].x, corners[i].y, corners[i].z);
				glVertex3f(corners[(i + 1) % 4].x, corners[(i + 1) % 4].y, corners[(i + 1) % 4].z);

				// Top face
				glVertex3f(corners[i + 4].x, corners[i + 4].y, corners[i + 4].z);
				glVertex3f(corners[((i + 1) % 4) + 4].x, corners[((i + 1) % 4) + 4].y, corners[((i + 1) % 4) + 4].z);

				// Connections between top and bottom faces
				glVertex3f(corners[i].x, corners[i].y, corners[i].z);
				glVertex3f(corners[i + 4].x, corners[i + 4].y, corners[i + 4].z);
			}

			glEnd();
		}
	}
}