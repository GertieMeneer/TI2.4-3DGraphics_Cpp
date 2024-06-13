#pragma once
#include "Entity.h"
#include "ModelComponent.h"
#include "cam.h"
#include <vector>

class Game {
public:
	std::vector<std::unique_ptr<Entity>> entities;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
	GLFWwindow* window;
	cam* camera;
	ModelComponent* model;

	Game();
	~Game();

	void init(cam *camera, GLFWwindow *window);
	void run(float deltaTime);
	void draw();

	void updateParticles(float deltaTime);

	bool checkCollision(Entity& a, Entity& b);
	void mouseButtonCallback(int button, int action, int mods);

private:
	float powerupTimer = 0.0f;
	bool canShoot = false;

	//particle spawn properties
	float spawnHeight = 20.0f;
	float spawnTimerThreshold = 0.1f;
	float spawnTimer = 0.0f;

	//particle speed/lifetime properties
	float particleSpeed = 10.0f;
	float particleScale = 0.5f;
	float particleLifetime = 10.0f;
};
