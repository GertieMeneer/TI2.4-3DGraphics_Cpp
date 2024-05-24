#include "tigl.h"
#include "cam.h"
#include "Util.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <random>
#include <chrono>

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
cam* camera;

// ECS Components
struct TransformComponent {
    glm::vec3 position;
    glm::vec3 scale;
};

struct VelocityComponent {
    glm::vec3 velocity;
};

struct RenderableComponent {
    std::vector<Vertex> vertices;
};

struct ColliderComponent {
    float radius;
};

struct LifetimeComponent {
    float lifetime;
};

// Entity
class Entity {
public:
    std::unique_ptr<TransformComponent> transform;
    std::unique_ptr<VelocityComponent> velocity;
    std::unique_ptr<RenderableComponent> renderable;
    std::unique_ptr<ColliderComponent> collider;
    std::unique_ptr<LifetimeComponent> lifetime;

    Entity() :
        transform(std::make_unique<TransformComponent>()),
        velocity(nullptr),
        renderable(nullptr),
        collider(nullptr),
        lifetime(nullptr) {}
};

// System Prototypes
void init();
void update(float deltaTime);
void draw();
void spawnParticles(float deltaTime);
void moveEntities(float deltaTime);
void renderEntities();
bool checkCollision(const Entity& a, const Entity& b);

// Globals
std::vector<std::unique_ptr<Entity>> entities;
Entity* player;

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-10.0f, 10.0f);
std::uniform_real_distribution<float> y_distribution(0.5f, 10.0f);

std::chrono::time_point<std::chrono::steady_clock> startTime;

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glfw";

    window = glfwCreateWindow(1280, 720, "Shooting Game", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glfw";
    }
    glfwMakeContextCurrent(window);

    tigl::init();
    init();

    float lastTime = (float)glfwGetTime();
    startTime = std::chrono::steady_clock::now();

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
    int value[10];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, true);
        });

    camera = new cam(window);

    // Initialize player
    player = new Entity();
    player->transform->position = glm::vec3(0, 0, 0);
    player->collider = std::make_unique<ColliderComponent>();
    player->collider->radius = 0.5f; // Adjust as needed

    entities.push_back(std::unique_ptr<Entity>(player));

    // Create the floor
    auto floor = std::make_unique<Entity>();
    floor->transform->position = glm::vec3(0, -1, 0);
    floor->renderable = std::make_unique<RenderableComponent>();
    floor->renderable->vertices = Util::buildFloor();

    entities.push_back(std::move(floor));
}

void update(float deltaTime)
{
    camera->update(window, deltaTime);
    spawnParticles(deltaTime);
    moveEntities(deltaTime);

    // Check collisions
    for (auto& entity : entities)
    {
        if (entity.get() != player && entity->collider && checkCollision(*player, *entity))
        {
            std::cout << "Collision detected! Game Over!" << std::endl;
            glfwSetWindowShouldClose(window, true);
        }
    }

    // Remove particles that have exceeded their lifetime
    auto currentTime = std::chrono::steady_clock::now();
    for (auto it = entities.begin(); it != entities.end();)
    {
        if ((*it)->lifetime && std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= (*it)->lifetime->lifetime)
        {
            it = entities.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    tigl::shader->enableColor(true);

    glEnable(GL_DEPTH_TEST);

    renderEntities();

    glDisable(GL_DEPTH_TEST);
}

void spawnParticles(float deltaTime)
{
    // Spawn particles every 2 seconds
    static float spawnTimer = 0.0f;
    spawnTimer += deltaTime;
    if (spawnTimer >= 2.0f)
    {
        for (int i = 0; i < 10; ++i) // Spawn 10 particles for example
        {
            float x = distribution(generator);
            float y = y_distribution(generator);
            float z = distribution(generator);

            auto particle = std::make_unique<Entity>();
            particle->transform->position = glm::vec3(x, y, z);
            particle->transform->scale = glm::vec3(0.5f);
            particle->velocity = std::make_unique<VelocityComponent>();
            particle->velocity->velocity = glm::vec3(0, 0, 2.0f); // Move towards the player

            particle->renderable = std::make_unique<RenderableComponent>();
            particle->renderable->vertices = Util::buildCube(particle->transform->position, particle->transform->scale, glm::vec4(1, 0, 0, 1));

            particle->collider = std::make_unique<ColliderComponent>();
            particle->collider->radius = 0.5f; // Adjust as needed

            particle->lifetime = std::make_unique<LifetimeComponent>();
            particle->lifetime->lifetime = 30.0f; // Lifetime of 30 seconds

            entities.push_back(std::move(particle));
        }
        spawnTimer = 0.0f;
    }
}

void moveEntities(float deltaTime)
{
    for (auto& entity : entities)
    {
        if (entity->velocity)
        {
            entity->transform->position += entity->velocity->velocity * deltaTime;

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

void renderEntities()
{
    for (auto& entity : entities)
    {
        if (entity->renderable)
        {
            tigl::drawVertices(GL_QUADS, entity->renderable->vertices);
        }
    }
}

bool checkCollision(const Entity& a, const Entity& b)
{
    float distance = glm::length(a.transform->position - b.transform->position);
    return distance < (a.collider->radius + b.collider->radius);
}
