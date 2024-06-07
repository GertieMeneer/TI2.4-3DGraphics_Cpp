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
	auto player = std::make_unique<Entity>();
	player->playerComponent = new PlayerComponent(camera);
	player->colliderComponent = new ColliderComponent(glm::vec3(-0.5f), glm::vec3(0.5f));
	entities.push_back(std::move(player));
}

void Game::run(float deltaTime)
{
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
				std::cout << "Collision detected! Game Over!" << std::endl;
				//endTime = std::chrono::steady_clock::now();
				//Util::SaveScore(startTime, endTime, "collision with block");
				//close window
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
		tigl::drawVertices(GL_QUADS, entity->vertices);

		Entity* player = nullptr;
		if (entity->playerComponent) {
			player = entity.get();

			Util::drawPlayerColliderBoundsBox(player);

		}
	}

	Util::drawParticleColliderBoundsBox(entities);
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

bool Game::checkCollision(const Entity& a, const Entity& b)
{
	// check if both entities have colliders
	if (!&a.colliderComponent || !&b.colliderComponent)
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
