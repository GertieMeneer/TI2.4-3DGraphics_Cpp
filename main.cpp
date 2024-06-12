#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include "tigl.h"
#include "FileIO.h"
#include "Game.h"

using tigl::Vertex;

GLFWwindow* window;
Game* game;
std::unique_ptr<cam> camera;

void init();
void update(float deltaTime);
void draw();

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
				game->endTime = std::chrono::steady_clock::now();
				FileIO::saveScore(game->startTime, game->endTime, "pressed esc key");
				glfwSetWindowShouldClose(window, true);
			}

		});

	// enable depth
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);				// "sky" color

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(100.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);
	tigl::shader->setProjectionMatrix(projection);  
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColorMult(true);
	tigl::shader->enableColor(true);

	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightAmbient(0, glm::vec3(0.1f, 0.1f, 0.1f));
	tigl::shader->setLightDiffuse(0, glm::vec3(1.0f, 1.0f, 1.0f));
	tigl::shader->setLightSpecular(0, glm::vec3(1.0f, 1.0f, 1.0f));
	tigl::shader->setLightPosition(0, glm::vec3(10, 10, 10));
	tigl::shader->setShinyness(100);

	//tigl::shader->enableFog(true);
	//tigl::shader->setFogExp(0.1f);

	game = new Game();
	camera = std::make_unique<cam>(window);

	game->init(*camera, *window);
}

void update(float deltaTime)
{
	camera->update(window, deltaTime);
	game->run(deltaTime);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	tigl::shader->setViewMatrix(camera->getMatrix());
	game->draw();
}
