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
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <random>
#include <chrono>

//usings
using tigl::Vertex;

//globals
GLFWwindow* window;
cam* camera;

std::vector<std::unique_ptr<Entity>> entities;		//list of entities
Entity* player;
Entity* floorEntity;

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-5.0f, 5.0f);
std::uniform_real_distribution<float> y_distribution(0.5f, 10.0f);

//function declerations
void init();
void update(float deltaTime);
void draw();
void spawnParticles(float deltaTime);
void moveEntities(float deltaTime);
void renderEntities();
bool checkCollision(const Entity& a, const Entity& b);
void drawParticleColliderBoundsBox();
void renderFloor();

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glfw";

	//create new window
	window = glfwCreateWindow(1600, 980, "Shooting Game", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glfw";
	}
	//add glfw context to windows
	glfwMakeContextCurrent(window);

	//init tigl lib and game
	tigl::init();
	init();

	float lastTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		update(deltaTime);			//update stuff
		draw();						//draw stuff

		glfwSwapBuffers(window);	//swap buffers
		glfwPollEvents();			//keyboard/mouse events
	}

	glfwTerminate();		//exit when window closes
	return 0;
}

void init()
{
	//get and set max texture size
	int value[10];
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);

	//add esc key callback to close window
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});

	//enable depth
	glEnable(GL_DEPTH_TEST);

	//init cam
	camera = new cam(window);

	//init player
	player = new Entity();
	player->transform.position = glm::vec3(0, 1, 0);			//set player position
	player->collider = std::make_unique<ColliderComponent>();	//add collider component

	//set bouding box player collider
	float playerSize = 1.0f;
	player->collider->minBounds = glm::vec3(-playerSize / 2.0f);
	player->collider->maxBounds = glm::vec3(playerSize / 2.0f);

	entities.push_back(std::unique_ptr<Entity>(player));	//add player to entities list

	//init floor
	floorEntity = new Entity();
	floorEntity->transform.position = glm::vec3(0, 0, 0);				//set floor position
	floorEntity->renderable = std::make_unique<RenderableComponent>();	//add renderable component
	floorEntity->renderable->vertices = Util::buildFloor();				//add floor vertices to renderable component

	entities.push_back(std::unique_ptr<Entity>(floorEntity));	//add floor to entities list
}

void update(float deltaTime)
{
	camera->update(window, deltaTime);		//update cam
	player->transform.position = camera->getPosition(); //player position to cam position

	//std::cout << "Player position: " << player->transform->position.x << ", " << player->transform->position.y << ", " << player->transform->position.z << std::endl;

	spawnParticles(deltaTime);
	moveEntities(deltaTime);

	//check collisions
	for (auto& entity : entities)
	{
		if (entity.get() != player && entity->collider && checkCollision(*player, *entity))
		{
			std::cout << "Collision detected! Game Over!" << std::endl;
			glfwSetWindowShouldClose(window, true);
		}
	}

	//update particle lifetimes, remove expired particles
	auto currentTime = std::chrono::steady_clock::now();
	auto it = entities.begin();
	while (it != entities.end())
	{
		if ((*it)->lifetime)
		{
			auto spawnTime = (*it)->lifetime->spawnTime;
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - spawnTime);
			auto timeAlive = duration.count();
			if (timeAlive >= (*it)->lifetime->lifetime)
			{
				it = entities.erase(it); //remove particle
				continue;
			}
		}
		++it;
	}
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);				//"sky" color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear color/depth

	//get and set viewport
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(100.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(camera->getMatrix());
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableColor(true);

	//draw collider box around player
	//Util::drawPlayerColliderBoundsBox(player, camera);

	//draw collider box around cube
	drawParticleColliderBoundsBox();

	//draw entities
	for (auto& entity : entities)
	{
		if (entity->renderable)
		{
			tigl::drawVertices(GL_QUADS, entity->renderable->vertices);
		}
	}
}

void drawParticleColliderBoundsBox()
{
	for (const auto& entityPtr : entities) {
		const Entity& entity = *entityPtr;
		if (&entity != player && entity.collider) {
			// Get particle's collider bounds
			glm::vec3 minBounds = entity.collider->minBounds + entity.transform.position;
			glm::vec3 maxBounds = entity.collider->maxBounds + entity.transform.position;

			// Calculate the corners of the box relative to the particle's position
			glm::vec3 corners[8] = {
				minBounds,
				glm::vec3(maxBounds.x, minBounds.y, minBounds.z),
				glm::vec3(maxBounds.x, minBounds.y, maxBounds.z),
				glm::vec3(minBounds.x, minBounds.y, maxBounds.z),
				glm::vec3(minBounds.x, maxBounds.y, minBounds.z),
				glm::vec3(maxBounds.x, maxBounds.y, minBounds.z),
				maxBounds,
				glm::vec3(minBounds.x, maxBounds.y, maxBounds.z)
			};

			// Draw the box using GL_LINES
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f); // Set color to white

			// Draw lines between corners to form the edges of the box
			for (int i = 0; i < 4; ++i) {
				// Bottom face
				glVertex3f(corners[i].x, corners[i].y, corners[i].z);
				glVertex3f(corners[(i + 1) % 4].x, corners[(i + 1) % 4].y, corners[(i + 1) % 4].z);

				// Top face
				glVertex3f(corners[i + 4].x, corners[i + 4].y, corners[i + 4].z);
				glVertex3f(corners[((i + 1) % 4) + 4].x, corners[((i + 1) % 4) + 4].y, corners[((i + 1) % 4) + 4].z);

				// Connections between top and bottom faces
				glVertex3f(corners[i].x, corners[i].y, corners[i].z);
				glVertex3f(corners[i + 4].x, corners[i + 4].y, corners[i + 4].z);
			}

			glEnd();
		}
	}
}

void spawnParticles(float deltaTime)
{
	// Spawn a particle every 0.5 seconds
	static float spawnTimer = 0.0f;
	spawnTimer += deltaTime;
	if (spawnTimer >= 0.1f) // Adjust spawn rate as needed
	{
		float x = distribution(generator);
		float z = distribution(generator);

		auto particle = std::make_unique<Entity>();
		particle->transform.position = glm::vec3(x, 10.0f, z); // Start position at a height
		particle->transform.scale = glm::vec3(0.5f);

		// Set direction to move downwards
		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

		particle->velocity = std::make_unique<VelocityComponent>();
		particle->velocity->velocity = direction * 10.0f; // Adjust speed as needed

		particle->renderable = std::make_unique<RenderableComponent>();
		particle->renderable->vertices = Util::buildCube(particle->transform.position, particle->transform.scale, glm::vec4(1, 0, 0, 1));

		particle->collider = std::make_unique<ColliderComponent>();

		// Set bounding box for particle collider
		float particleSize = 0.5f; // Should match particle scale
		particle->collider->minBounds = glm::vec3(-particleSize);
		particle->collider->maxBounds = glm::vec3(particleSize);

		particle->lifetime = std::make_unique<LifetimeComponent>();
		particle->lifetime->lifetime = 10.0f; // Lifetime of 10 seconds
		particle->lifetime->spawnTime = std::chrono::steady_clock::now(); // Record spawn time

		entities.push_back(std::move(particle));
		spawnTimer = 0.0f;
	}
}

void moveEntities(float deltaTime)
{
	for (auto& entity : entities)
	{
		if (entity->velocity)
		{
			entity->transform.position += entity->velocity->velocity * deltaTime;

			// Update the vertices position for rendering
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

bool checkCollision(const Entity& a, const Entity& b)
{
	// Check if both entities have colliders
	if (!a.collider || !b.collider)
		return false;

	// Convert collider bounds to global coordinates
	glm::vec3 aMinBoundsGlobal = a.transform.position + a.collider->minBounds;
	glm::vec3 aMaxBoundsGlobal = a.transform.position + a.collider->maxBounds;
	glm::vec3 bMinBoundsGlobal = b.transform.position + b.collider->minBounds;
	glm::vec3 bMaxBoundsGlobal = b.transform.position + b.collider->maxBounds;

	// Check for overlap along each axis
	bool overlapX = (aMinBoundsGlobal.x <= bMaxBoundsGlobal.x && aMaxBoundsGlobal.x >= bMinBoundsGlobal.x);
	bool overlapY = (aMinBoundsGlobal.y <= bMaxBoundsGlobal.y && aMaxBoundsGlobal.y >= bMinBoundsGlobal.y);
	bool overlapZ = (aMinBoundsGlobal.z <= bMaxBoundsGlobal.z && aMaxBoundsGlobal.z >= bMinBoundsGlobal.z);

	// If there's overlap along all axes, then there's a collision
	return overlapX && overlapY && overlapZ;
}
