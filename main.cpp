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
	camera->update(window, deltaTime);
	game->run(deltaTime);
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



	game->draw();
}
