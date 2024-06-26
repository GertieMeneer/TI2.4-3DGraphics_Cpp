#define PI 3.14159265358979323846
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

std::vector<Vertex> Util::drawEntityCollider(Entity* entity)
{
	glm::vec3 minBounds = entity->position - glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 maxBounds = entity->position + glm::vec3(0.5f, 0.5f, 0.5f);

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

std::vector<Vertex> Util::drawCrosshair(glm::vec3 position) {
	std::vector<Vertex> verts;

	glm::vec3 minBounds = position - glm::vec3(10.0f, 10.0f, 0.0f);
	glm::vec3 maxBounds = position + glm::vec3(10.0f, 10.0f, 0.0f);

	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, 0.0f), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, 0.0f), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, 0.0f), glm::vec4(1, 1, 1, 1)));
	verts.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, 0.0f), glm::vec4(1, 1, 1, 1)));



	return verts;
}

/**
* Replaces a substring in a string
*/
std::string Util::replace(std::string str, const std::string& toReplace, const std::string& replacement)
{
	size_t index = 0;
	while (true)
	{
		index = str.find(toReplace, index);
		if (index == std::string::npos)
			break;
		str.replace(index, toReplace.length(), replacement);
		++index;
	}
	return str;
}

/**
* Splits a string into substrings, based on a seperator
*/
std::vector<std::string> Util::split(std::string str, const std::string& seperator)
{
	std::vector<std::string> ret;
	size_t index;
	while (true)
	{
		index = str.find(seperator);
		if (index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index + 1);
	}
	ret.push_back(str);
	return ret;
}

/**
* Turns a string to lowercase
*/
std::string Util::toLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

/**
* Cleans up a line for processing
*/
std::string Util::cleanLine(std::string line)
{
	line = replace(line, "\t", " ");
	while (line.find("  ") != std::string::npos)
		line = replace(line, "  ", " ");
	if (line == "")
		return "";
	if (line[0] == ' ')
		line = line.substr(1);
	if (line == "")
		return "";
	if (line[line.length() - 1] == ' ')
		line = line.substr(0, line.length() - 1);
	return line;
}
