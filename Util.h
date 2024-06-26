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
	static std::vector<Vertex> drawEntityCollider(Entity* particle);
	static std::vector<Vertex> drawCrosshair(glm::vec3(position));
	static std::string replace(std::string str, const std::string& toReplace, const std::string& replacement);
	static std::vector<std::string> split(std::string str, const std::string& seperator);
	static std::string toLower(std::string data);
	static std::string cleanLine(std::string line);
};
