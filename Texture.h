#pragma once

#include <gl/glew.h>
#include <string>
#include <vector>
#include "glm/glm.hpp"

class Texture
{
	GLuint id;
	std::vector<unsigned char> colorData;
	int width, height;
public:
	Texture(const std::string &fileName);
	void bind();
};
