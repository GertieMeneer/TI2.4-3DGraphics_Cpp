#include "Game.h"

#include <random>
#include "util.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::init(cam& camera)
{
	player = new Entity();
	player->playerComponent = new PlayerComponent(camera);
	player->colliderComponent = new ColliderComponent(glm::vec3(-0.5f), glm::vec3(0.5f));
}

void Game::run(float deltaTime)
{
	for (auto& entity : entities) {
		entity->update(deltaTime);
	}

	updateParticles(deltaTime);
}

void Game::draw()
{

}

void Game::updateParticles(float deltaTime)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(),
		[](const std::unique_ptr<Entity>& entity) { // Change from shared_ptr to unique_ptr
			return entity->toBeRemoved;
		}),
		entities.end());

	float spawnHeight = 10.0f;
	float spawnTimerThreshold = 0.1f;
	static float spawnTimer = 0.0f;

	float particleSpeed = 10.0f;
	float particleScale = 0.5f;
	float particleLifetime = 10.0f;
	std::uniform_real_distribution<float> distribution(-5.0f, 5.0f);		// random float in range
	std::random_device rd;
	std::mt19937 generator(rd());

	spawnTimer += deltaTime;
	if (spawnTimer >= spawnTimerThreshold)
	{
		float randomX = distribution(generator);		// generate random float for x coordinate
		float randomZ = distribution(generator);		// generate random float for z coordinate

		// create particle
		auto particle = std::make_unique<Entity>();
		particle->position = glm::vec3(randomX, spawnHeight, randomZ);

		//particle->transform.scale = glm::vec3(particleScale);

		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);		// moving downwards direction

		particle->velocityComponent = new VelocityComponent(direction * particleSpeed);
		particle->vertices = Util::buildCube(particle->position, glm::vec3(particleScale), glm::vec4(1, 0, 0, 1));
		particle->colliderComponent = new ColliderComponent(glm::vec3(-particleScale), glm::vec3(particleScale));
		particle->lifetimeComponent = new LifetimeComponent(particleLifetime, std::chrono::steady_clock::now());

		entities.push_back(std::move(particle));	// add particle to list of entities
		spawnTimer = 0.0f;	// set spawn timer back to 0
	}
}