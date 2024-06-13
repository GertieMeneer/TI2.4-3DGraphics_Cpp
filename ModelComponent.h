#pragma once

#include "tigl.h"
#include "Texture.h"
#include "Component.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <algorithm>

using tigl::Vertex;

class Texture;

class ObjModel : public Component
{
private:
	float amountToMove;

	class Vertex
	{
	public:
		int position;
		int normal;
		int texcoord;
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
	};
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		Texture* texture;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		std::list<Face> faces;
	};


	std::vector<glm::vec3>	vertices;
	std::vector<glm::vec3>	normals;
	std::vector<glm::vec2>	texcoords;
	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;

	void loadMaterialFile(const std::string& fileName, const std::string& dirName);
public:
	std::vector<tigl::Vertex> verticesToDraw;

	glm::vec3 position;
	float rotationAngleY;
	float rotationAngleX;
	ObjModel(const std::string& filename);
	~ObjModel(void);

	virtual void update(float deltaTime) override;
	void draw();
};

