#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include "FileIO.h"
#include "Game.h"
#include "Util.h"

using tigl::Vertex;

GLFWwindow* window;
Game* game;
cam* camera;

int windowWith = 1400;
int windowHeight = 800;

float centerX;
float centerY;

float fov = 100.0f;

void init();
void update(float deltaTime);
void draw();
void drawCrosshair();

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glfw";

	window = glfwCreateWindow(windowWith, windowHeight, "Cube Cascade", NULL, NULL);
	centerX = windowWith / 2.0f;
	centerY = windowHeight / 2.0f;

	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glfw";
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);		//vsync, 1=enable, 0=disable

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
				FileIO::saveScore(game->startTime, game->endTime, "pressed ESC key");
				glfwSetWindowShouldClose(window, true);
			}
		});

	glEnable(GL_DEPTH_TEST);			//enable depth
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);				// "sky" color

	tigl::shader->setModelMatrix(glm::mat4(1.0f));		//4x4 matrix for model, transofmrations: translation, rotation, scaling, shearing
	tigl::shader->enableLighting(true);
	tigl::shader->setLightDiffuse(0, glm::vec3(1.0f, 1.0f, 1.0f));
	tigl::shader->setLightSpecular(0, glm::vec3(1.0f, 1.0f, 1.0f));
	tigl::shader->setLightPosition(0, glm::vec3(10, 20, 10));
	tigl::shader->setShinyness(100);


	//this is to enable fog, and set the intensity
	//does mess with my cube hitting algorithm so for now: disabled
	//doesn't really add anything to my game anyway :)
	/*tigl::shader->enableFog(true);
	tigl::shader->setFogExp(0.01f);*/

	game = new Game();
	camera = new cam(window);

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			game->mouseButtonCallback(button, action, mods);
		});

	game->init(camera, window);
}

void update(float deltaTime)
{
	camera->update(window, deltaTime);
	game->run(deltaTime);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear previous scene

	drawCrosshair();

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(fov), viewport[2] / (float)viewport[3], 0.01f, 100.0f);		//fov, aspect ratio, near/far clipping planes
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(camera->getMatrix());

	game->draw();
}

void drawCrosshair()
{
	glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(windowWith), static_cast<float>(windowHeight), 0.0f);
	tigl::shader->setProjectionMatrix(ortho);
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::drawVertices(GL_LINES, Util::drawCrosshair(centerX, centerY));

	glEnable(GL_DEPTH_TEST);
}
