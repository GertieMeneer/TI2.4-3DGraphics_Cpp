#include "Game.h"

#include <random>
#include <iostream>
#include "FileIO.h"
#include "Util.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::init(cam* camera, GLFWwindow* win, float x, float y)
{
	window = win;
	centerX = x;
	centerY = y;

	auto player = std::make_unique<Entity>();
	player->playerComponent = new PlayerComponent(camera);
	player->colliderComponent = new ColliderComponent(glm::vec3(-0.5f), glm::vec3(0.5f));
	player->name = "Player";
	entities.push_back(std::move(player));

	auto flag = std::make_unique<Entity>();
	flag->modelComponent = new ModelComponent("res/flag/Flag.obj");
	flag->name = "Flag";
	entities.push_back(std::move(flag));

	startTime = std::chrono::steady_clock::now();
}

void Game::run(float deltaTime)
{
	powerupTimer += deltaTime;
	if (powerupTimer >= 10.0f && powerupTimer < 10.2f) {		// to reduce stuttering and lag
		std::string title = "Power Up Available!";
		glfwSetWindowTitle(window, title.c_str());
		canShoot = true;
	}
	else if (powerupTimer < 10.0f) {
		canShoot = false;
	}

	for (auto& entity : entities) {
		entity->update(deltaTime);
	}

	Entity* player = nullptr;
	for (auto& entity : entities) {
		if (entity->playerComponent) {
			player = entity.get();
			break;
		}
	}

	if (player) {
		for (auto& entity : entities) {
			if (entity.get() != player && checkCollision(*player, *entity)) {
				std::cout << "Collision detected! Game Over :(" << std::endl;
				endTime = std::chrono::steady_clock::now();
				FileIO::saveScore(startTime, endTime, "collision with block");
				glfwSetWindowShouldClose(window, true);
				break;
			}
		}
	}

	updateParticles(deltaTime);
}

void Game::draw()
{
	tigl::drawVertices(GL_QUADS, Util::buildFloor());

	for (auto& entity : entities) {
		entity->draw();
		//tigl::drawVertices(GL_LINES, Util::drawEntityCollider(entity.get()));
	}
}

void Game::updateParticles(float deltaTime)
{
	//remove all entities where property toBeRemoved = true
	entities.erase(std::remove_if(entities.begin(), entities.end(),
		[](std::unique_ptr<Entity>& entity) {
			return entity->toBeRemoved;
		}),
		entities.end());

	std::uniform_real_distribution<float> distribution(-5.0f, 5.0f);		// random float in range
	std::random_device rd;
	std::mt19937 generator(rd());

	spawnTimer += deltaTime;
	if (spawnTimer >= spawnTimerThreshold)		//spawn new particle
	{
		float randomX = distribution(generator);		// generate random float for x coordinate
		float randomZ = distribution(generator);		// generate random float for z coordinate

		// create particle
		auto particle = std::make_unique<Entity>();
		particle->position = glm::vec3(randomX, spawnHeight, randomZ);
		particle->texture = new Texture("res/cube_texture.png");
		particle->name = "Cube";

		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);		// moving downwards direction

		particle->velocityComponent = new VelocityComponent(direction * particleSpeed);
		particle->vertices = Util::buildCube(particle->position, glm::vec3(particleScale));
		particle->colliderComponent = new ColliderComponent(glm::vec3(-particleScale), glm::vec3(particleScale));
		particle->lifetimeComponent = new LifetimeComponent(particleLifetime, std::chrono::steady_clock::now());

		entities.push_back(std::move(particle));	// add particle to list of entities
		spawnTimer = 0.0f;	// set spawn timer back to 0
	}
}

bool Game::checkCollision(Entity& a, Entity& b)
{
	// check if both entities have colliders
	if (!a.colliderComponent || !b.colliderComponent)
		return false;

	// convert collider bounds to global coordinates
	glm::vec3 aMinBoundsGlobal = a.position + a.colliderComponent->minBounds;
	glm::vec3 aMaxBoundsGlobal = a.position + a.colliderComponent->maxBounds;
	glm::vec3 bMinBoundsGlobal = b.position + b.colliderComponent->minBounds;
	glm::vec3 bMaxBoundsGlobal = b.position + b.colliderComponent->maxBounds;

	// check for overlap along each axis
	bool overlapX = (aMinBoundsGlobal.x <= bMaxBoundsGlobal.x && aMaxBoundsGlobal.x >= bMinBoundsGlobal.x);
	bool overlapY = (aMinBoundsGlobal.y <= bMaxBoundsGlobal.y && aMaxBoundsGlobal.y >= bMinBoundsGlobal.y);
	bool overlapZ = (aMinBoundsGlobal.z <= bMaxBoundsGlobal.z && aMaxBoundsGlobal.z >= bMinBoundsGlobal.z);

	// overlap if all axis's overlap
	return overlapX && overlapY && overlapZ;
}

void Game::mouseButtonCallback(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && canShoot)
	{
		// read center screen pixel, store in array
		unsigned char pixel[3];
		glReadPixels(centerX, centerY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

		//std::cout << "color rgb: " << static_cast<int>(pixel[0]) << ", " << static_cast<int>(pixel[1]) << ", " << static_cast<int>(pixel[2]) << std::endl;

		// check if color matches
		//25 = blue sky
		//>140 = floor
		for (size_t i = 0; i < entities.size(); ++i) {
			auto& entity = entities[i];
			if (!entity->playerComponent && !entity->modelComponent)
			{
				if ((pixel[0] != 25 && pixel[0] < 140) &&
					(pixel[1] != 25 && pixel[1] < 140) &&
					(pixel[2] != 25 && pixel[2] < 140)) {
					entity->toBeRemoved = true;
					std::string title = "Recharging Power Up...";
					glfwSetWindowTitle(window, title.c_str());
					powerupTimer = 0.0f;
				}
			}
		}
	}
}
