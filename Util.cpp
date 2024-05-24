#include <cmath>

#include "Util.h"
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
	verts.push_back(Vertex::PC(glm::vec3(-10, -1, -10), glm::vec4(1, 1, 1, 1))); // Bottom-left
	verts.push_back(Vertex::PC(glm::vec3(10, -1, -10), glm::vec4(1, 1, 1, 1)));  // Bottom-right
	verts.push_back(Vertex::PC(glm::vec3(10, -1, 10), glm::vec4(1, 1, 1, 1)));   // Top-right

	// Triangle 2
	verts.push_back(Vertex::PC(glm::vec3(-10, -1, -10), glm::vec4(1, 1, 1, 1))); // Bottom-left
	verts.push_back(Vertex::PC(glm::vec3(10, -1, 10), glm::vec4(1, 1, 1, 1)));   // Top-right
	verts.push_back(Vertex::PC(glm::vec3(-10, -1, 10), glm::vec4(1, 1, 1, 1)));  // Top-left

	// Additional triangle to cover the remaining part of the square
	// Triangle 3
	verts.push_back(Vertex::PC(glm::vec3(-10, -1, -10), glm::vec4(1, 1, 1, 1))); // Bottom-left
	verts.push_back(Vertex::PC(glm::vec3(10, -1, -10), glm::vec4(1, 1, 1, 1)));  // Bottom-right
	verts.push_back(Vertex::PC(glm::vec3(-10, -1, 10), glm::vec4(1, 1, 1, 1)));  // Top-left

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