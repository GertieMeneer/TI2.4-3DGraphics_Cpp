#pragma once
#include "Entity.h"
#include <vector>

class Game {
public:
	std::vector<std::unique_ptr<Entity>> entities;

	Game();
	~Game();

	void init(cam& camera);
	void run(float deltaTime);
	void draw();

	void updateParticles(float deltaTime);

	bool checkCollision(const Entity& a, const Entity& b);
};