#pragma once
#include "Entity.h"
#include "ObjModel.h"
#include <vector>

class Game {
public:
	std::vector<std::unique_ptr<Entity>> entities;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
	GLFWwindow* window;
	ObjModel* model;


	Game();
	~Game();

	void init(cam& camera, GLFWwindow& window);
	void run(float deltaTime);
	void draw();

	void updateParticles(float deltaTime);

	bool checkCollision(const Entity& a, const Entity& b);

private:
};
