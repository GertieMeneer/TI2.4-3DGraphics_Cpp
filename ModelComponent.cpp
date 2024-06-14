#include "ModelComponent.h"
#include "Util.h"

ModelComponent::ModelComponent(const std::string& fileName)
{
	std::cout << "Loading " << fileName << std::endl;
	std::string dirName = fileName;
	if (dirName.rfind("/") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("/"));
	if (dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	if (fileName == dirName)
		dirName = "";

	std::ifstream pFile(fileName.c_str());

	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}

	ObjGroup* currentGroup = new ObjGroup();
	currentGroup->materialIndex = -1;

	while (!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = Util::cleanLine(line);
		if (line == "" || line[0] == '#')
			continue;

		std::vector<std::string> params = Util::split(line, " ");
		params[0] = Util::toLower(params[0]);

		if (params[0] == "v")
			vertices.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if (params[0] == "vn")
			normals.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if (params[0] == "vt")
			texcoords.push_back(glm::vec2((float)atof(params[1].c_str()), (float)atof(params[2].c_str())));
		else if (params[0] == "f")
		{
			for (size_t ii = 4; ii <= params.size(); ii++)
			{
				Face face;

				for (size_t i = ii - 3; i < ii; i++)
				{
					Vertex vertex;
					std::vector<std::string> indices = Util::split(params[i == (ii - 3) ? 1 : i], "/");
					if (indices.size() >= 1)
						vertex.position = atoi(indices[0].c_str()) - 1;
					if (indices.size() == 2)
						vertex.texcoord = atoi(indices[1].c_str()) - 1;
					if (indices.size() == 3)
					{
						if (indices[1] != "")
							vertex.texcoord = atoi(indices[1].c_str()) - 1;
						vertex.normal = atoi(indices[2].c_str()) - 1;
					}
					face.vertices.push_back(vertex);
				}
				currentGroup->faces.push_back(face);
			}
		}
		else if (params[0] == "mtllib")
		{
			loadMaterialFile(dirName + "/" + params[1], dirName);
		}
		else if (params[0] == "usemtl")
		{
			if (currentGroup->faces.size() != 0)
				groups.push_back(currentGroup);
			currentGroup = new ObjGroup();
			currentGroup->materialIndex = -1;

			for (size_t i = 0; i < materials.size(); i++)
			{
				MaterialInfo* info = materials[i];
				if (info->name == params[1])
				{
					currentGroup->materialIndex = i;
					break;
				}
			}
			if (currentGroup->materialIndex == -1)
				std::cout << "Could not find material name " << params[1] << std::endl;
		}
	}
	groups.push_back(currentGroup);
	amountToMove = 10.0f;
	rotationAngleY = 0.0f;
}

ModelComponent::~ModelComponent()
{

}

void ModelComponent::update(float deltaTime)
{
	if (position.y > 1.0f)
	{
		amountToMove = -1.0f;
	}
	if (position.y < -1.0f)
	{
		amountToMove = 1.0f;
	}
	position.y += amountToMove * deltaTime;
	rotationAngleY += 100.0f * deltaTime;
}

void ModelComponent::draw()
{
	const float scale = 0.1f;
	glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotation = rotationX * rotationY;

	glm::mat4 modelMatrix = translation * rotation * scaling;

	verticesToDraw.clear();

	for (auto& group : groups) {
		for (auto& face : group->faces) {
			for (auto& vertex : face.vertices) {
				glm::vec3 transformedPosition = glm::vec3(modelMatrix * glm::vec4(vertices[vertex.position], 1.0f));
				auto v = tigl::Vertex::PTN(transformedPosition, texcoords[vertex.texcoord], normals[vertex.normal]);
				verticesToDraw.push_back(v);
			}
		}
	}

	tigl::drawVertices(GL_TRIANGLES, verticesToDraw);
}

void ModelComponent::loadMaterialFile(const std::string& fileName, const std::string& dirName)
{
	std::cout << "Loading " << fileName << std::endl;
	std::ifstream pFile(fileName.c_str());
	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}

	MaterialInfo* currentMaterial = NULL;

	while (!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = Util::cleanLine(line);
		if (line == "" || line[0] == '#')
			continue;

		std::vector<std::string> params = Util::split(line, " ");
		params[0] = Util::toLower(params[0]);

		if (params[0] == "newmtl")
		{
			if (currentMaterial != NULL)
			{
				materials.push_back(currentMaterial);
			}
			currentMaterial = new MaterialInfo();
			currentMaterial->name = params[1];
		}
		else if (params[0] == "map_kd")
		{
			std::string tex = params[1];
			if (tex.find("/"))
				tex = tex.substr(tex.rfind("/") + 1);
			if (tex.find("\\"))
				tex = tex.substr(tex.rfind("\\") + 1);
		}
	}
	if (currentMaterial != NULL)
		materials.push_back(currentMaterial);

}

ModelComponent::MaterialInfo::MaterialInfo()
{
	texture = NULL;
}
