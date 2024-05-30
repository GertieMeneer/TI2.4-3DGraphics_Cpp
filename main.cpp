#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

//includes
#include "tigl.h"
#include "cam.h"
#include "Util.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "RenderableComponent.h"
#include "ColliderComponent.h"
#include "LifetimeComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <fstream>

// usings
using tigl::Vertex;

// globals
GLFWwindow* window;
std::unique_ptr<cam> camera;
std::chrono::steady_clock::time_point startTime;
std::chrono::steady_clock::time_point endTime;

std::vector<std::unique_ptr<Entity>> entities;		// list of entities
Entity* player;
Entity* floorEntity;

std::uniform_real_distribution<float> distribution(-5.0f, 5.0f);		// random float in range
std::default_random_engine generator;		// generator for random float

// function declerations
void init();
void update(float deltaTime);
void draw();
void spawnParticles(float deltaTime);
void moveEntities(float deltaTime);
bool checkCollision(const Entity& a, const Entity& b);

/// <summary>
/// Entry point
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glfw";

	// create new window
	window = glfwCreateWindow(1600, 980, "Cube Cascade", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glfw";
	}
	// add glfw context to windows
	glfwMakeContextCurrent(window);

	// init tigl lib and game
	tigl::init();
	init();

	startTime = std::chrono::steady_clock::now(); // Record start time

	float lastTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		update(deltaTime);			// update stuff
		draw();						// draw stuff

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// keyboard/mouse events
	}

	glfwTerminate();		// exit when window closes
	return 0;
}

/// <summary>
/// Sets up GL, camera, player, entities
/// </summary>
void init()
{
	// get and set max texture size
	//int value[10];
	//glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);

	// add esc key callback to close window
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE) {
				endTime = std::chrono::steady_clock::now();
				Util::SaveScore(startTime, endTime, "pressed esc key");
				glfwSetWindowShouldClose(window, true);
			}
				
		});

	// enable depth
	glEnable(GL_DEPTH_TEST);

	// init cam
	camera = std::make_unique<cam>(window);

	// init player
	player = new Entity();
	player->transform.position = glm::vec3(0, 1, 0); // set player position
	player->collider = std::make_unique<ColliderComponent>(); // add collider component

	// set bounding box player collider
	float playerSize = 1.0f;
	player->collider->minBounds = glm::vec3(-playerSize / 2.0f);
	player->collider->maxBounds = glm::vec3(playerSize / 2.0f);

	entities.push_back(std::unique_ptr<Entity>(player)); // move player to entities list

	// init floor
	floorEntity = new Entity();
	floorEntity->transform.position = glm::vec3(0, 0, 0); // set floor position
	floorEntity->renderable = std::make_unique<RenderableComponent>(); // add renderable component
	floorEntity->renderable->vertices = Util::buildFloor(); // add floor vertices to renderable component

	entities.push_back(std::unique_ptr<Entity>(floorEntity)); // move floor to entities list
}

/// <summary>
/// Updates cam and player position
/// Spawns particles/removes, moves entities
/// Checks for collision
/// </summary>
/// <param name="deltaTime"></param>
void update(float deltaTime)
{
	camera->update(window, deltaTime);		// update cam
	player->transform.position = camera->getPosition(); // player position to cam position

	// print player position
	//std::cout << "Player position: " << player->transform->position.x << ", " << player->transform->position.y << ", " << player->transform->position.z << std::endl;

	spawnParticles(deltaTime);
	moveEntities(deltaTime);

	// check collisions
	for (auto& entity : entities)
	{
		if (entity.get() != player && entity->collider && checkCollision(*player, *entity))
		{
			std::cout << "Collision detected! Game Over!" << std::endl;
			endTime = std::chrono::steady_clock::now();
			Util::SaveScore(startTime, endTime, "collision with block");
			glfwSetWindowShouldClose(window, true);
		}
	}

	// update particle lifetimes, remove expired particles
	auto currentTime = std::chrono::steady_clock::now();
	auto it = entities.begin();
	while (it != entities.end())
	{
		if ((*it)->lifetime)
		{
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - (*it)->lifetime->spawnTime);
			auto timeAlive = duration.count();
			if (timeAlive >= (*it)->lifetime->lifetime)
			{
				it = entities.erase(it);  // remove particle
				continue;
			}
		}
		++it;
	}
}

/// <summary>
/// Draws scene and all entities
/// </summary>
void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);				// "sky" color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear color/depth

	// get and set viewport
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(100.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(camera->getMatrix());
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableColor(true);

	// draw collider box around player
	//Util::drawPlayerColliderBoundsBox(player, camera);

	// draw collider box around cube
	//Util::drawParticleColliderBoundsBox(entities, player);

	// draw entities
	for (auto& entity : entities)
	{
		if (entity->renderable)
		{
			tigl::drawVertices(GL_QUADS, entity->renderable->vertices);
		}
	}
}

/// <summary>
/// Adds new particles using a timer
/// </summary>
/// <param name="deltaTime"></param>
void spawnParticles(float deltaTime)
{
	float spawnHeight = 10.0f;
	float spawnTimerThreshold = 0.1f;
	static float spawnTimer = 0.0f;

	float particleSpeed = 10.0f;
	float particleScale = 0.5f;
	float particleLifetime = 10.0f;

	spawnTimer += deltaTime;
	if (spawnTimer >= spawnTimerThreshold)
	{
		float randomX = distribution(generator);		// generate random float for x coordinate
		float randomZ = distribution(generator);		// generate random float for z coordinate

		// create particle
		auto particle = std::make_unique<Entity>();
		particle->transform.position = glm::vec3(randomX, spawnHeight, randomZ);
		particle->transform.scale = glm::vec3(particleScale);

		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);		// moving downwards direction

		particle->velocity = std::make_unique<VelocityComponent>();		// add velocity component
		particle->velocity->velocity = direction * particleSpeed;		// set particle speed

		particle->renderable = std::make_unique<RenderableComponent>();	// add renderable component
		particle->renderable->vertices = Util::buildCube(particle->transform.position, particle->transform.scale, glm::vec4(1, 0, 0, 1));	// add cube vertices to particle

		particle->collider = std::make_unique<ColliderComponent>();		// add collider component

		// set particle collider bounding box
		particle->collider->minBounds = glm::vec3(-particleScale);
		particle->collider->maxBounds = glm::vec3(particleScale);

		particle->lifetime = std::make_unique<LifetimeComponent>();
		particle->lifetime->lifetime = particleLifetime;					// set particle lifetime
		particle->lifetime->spawnTime = std::chrono::steady_clock::now();	// set spawn time

		entities.push_back(std::move(particle));	// add particle to list of entities
		spawnTimer = 0.0f;	// set spawn timer back to 0
	}
}

/// <summary>
/// Changes the position of the entities that have a velocity
/// </summary>
/// <param name="deltaTime"></param>
void moveEntities(float deltaTime)
{
	for (auto& entity : entities)
	{
		if (entity->velocity)
		{
			entity->transform.position += entity->velocity->velocity * deltaTime;

			// update vertices position for rendering
			if (entity->renderable)
			{
				for (auto& vertex : entity->renderable->vertices)
				{
					vertex.position += entity->velocity->velocity * deltaTime;
				}
			}
		}
	}
}

/// <summary>
/// Checks if there is a collision between two entities
/// Does so by checking for overlap along x, y, z axis
/// </summary>
/// <param name="a">Entity A</param>
/// <param name="b">Entity B</param>
/// <returns>boolean (collision yes or no)</returns>
bool checkCollision(const Entity& a, const Entity& b)
{
	// check if both entities have colliders
	if (!a.collider || !b.collider)
		return false;

	// convert collider bounds to global coordinates
	glm::vec3 aMinBoundsGlobal = a.transform.position + a.collider->minBounds;
	glm::vec3 aMaxBoundsGlobal = a.transform.position + a.collider->maxBounds;
	glm::vec3 bMinBoundsGlobal = b.transform.position + b.collider->minBounds;
	glm::vec3 bMaxBoundsGlobal = b.transform.position + b.collider->maxBounds;

	// check for overlap along each axis
	bool overlapX = (aMinBoundsGlobal.x <= bMaxBoundsGlobal.x && aMaxBoundsGlobal.x >= bMinBoundsGlobal.x);
	bool overlapY = (aMinBoundsGlobal.y <= bMaxBoundsGlobal.y && aMaxBoundsGlobal.y >= bMinBoundsGlobal.y);
	bool overlapZ = (aMinBoundsGlobal.z <= bMaxBoundsGlobal.z && aMaxBoundsGlobal.z >= bMinBoundsGlobal.z);

	// overlap if all axis's overlap
	return overlapX && overlapY && overlapZ;
}
