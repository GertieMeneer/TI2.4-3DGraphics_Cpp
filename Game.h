#pragma once
#include "Entity.h"
#include <vector>

class Game {
public:
	std::vector<std::unique_ptr<Entity>> entities;
	Entity* player;
	Entity* floor;

	Game();
	~Game();

	void init(cam& camera);
	void run(float deltaTime);
	void draw();

	void updateParticles(float deltaTime);
};
