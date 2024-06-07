#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Util.h"
#include "Game.h"

#include <chrono>

using tigl::Vertex;

GLFWwindow* window;
Game* game;
std::unique_ptr<cam> camera;
std::chrono::steady_clock::time_point startTime;
std::chrono::steady_clock::time_point endTime;

void init();
void update(float deltaTime);
void draw();
//bool checkCollision(const Entity& a, const Entity& b);

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glfw";

	window = glfwCreateWindow(1600, 980, "Cube Cascade", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glfw";
	}
	glfwMakeContextCurrent(window);

	tigl::init();
	init();

	startTime = std::chrono::steady_clock::now(); // Record start time

	float lastTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		update(deltaTime);
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

/// <summary>
/// Sets up GL, camera, player, entities
/// </summary>
void init()
{
	// get and set max texture size
	int value[10];
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);

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
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);				// "sky" color

	// init game
	game = new Game();

	// init cam
	camera = std::make_unique<cam>(window);
	game->init(*camera);

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
	game->run(deltaTime);
	//for (auto &entity : game->entities) {
	//	entity->update(deltaTime, *entity, *camera);
	//	if (entity.get() != player && checkCollision(*player, *entity))
	//	{
	//		std::cout << "Collision detected! Game Over!" << std::endl;
	//		endTime = std::chrono::steady_clock::now();
	//		Util::SaveScore(startTime, endTime, "collision with block");
	//		glfwSetWindowShouldClose(window, true);
	//	}
	//}

	// print player position
	//std::cout << "Player position: " << player->transform->position.x << ", " << player->transform->position.y << ", " << player->transform->position.z << std::endl;
}

/// <summary>
/// Draws scene and all entities
/// </summary>
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear color/depth

	// get and set viewport
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(100.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(camera->getMatrix());
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableColor(true);

#ifdef DEBUG
	// draw collider box around player
	//Util::drawPlayerColliderBoundsBox(player, camera);

	// draw collider box around cube
	//Util::drawParticleColliderBoundsBox(entities, player);
#endif // DEBUG

	game->draw();
}

/// <summary>
/// Checks if there is a collision between two entities
/// Does so by checking for overlap along x, y, z axis
/// </summary>
/// <param name="a">Entity A</param>
/// <param name="b">Entity B</param>
/// <returns>boolean (collision yes or no)</returns>
//bool checkCollision(Entity& a, Entity& b)
//{
//	ColliderComponent* colComp = nullptr;
//
//	for (Component* comp : a.components) {
//		colComp = dynamic_cast<ColliderComponent*>(comp);
//		if (colComp != nullptr) {
//			break;
//		}
//
//	}
//
//	// convert collider bounds to global coordinates
//	glm::vec3 aMinBoundsGlobal = a.position + colComp->minBounds;
//	glm::vec3 aMaxBoundsGlobal = a.position + colComp->maxBounds;
//	glm::vec3 bMinBoundsGlobal = b.position + colComp->minBounds;
//	glm::vec3 bMaxBoundsGlobal = b.position + colComp->maxBounds;
//
//	// check for overlap along each axis
//	bool overlapX = (aMinBoundsGlobal.x <= bMaxBoundsGlobal.x && aMaxBoundsGlobal.x >= bMinBoundsGlobal.x);
//	bool overlapY = (aMinBoundsGlobal.y <= bMaxBoundsGlobal.y && aMaxBoundsGlobal.y >= bMinBoundsGlobal.y);
//	bool overlapZ = (aMinBoundsGlobal.z <= bMaxBoundsGlobal.z && aMaxBoundsGlobal.z >= bMinBoundsGlobal.z);
//
//	// overlap if all axis's overlap
//	return overlapX && overlapY && overlapZ;
//}
