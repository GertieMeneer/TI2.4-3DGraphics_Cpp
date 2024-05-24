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
    glm::vec3 minBounds; // Minimum coordinates of the bounding box
    glm::vec3 maxBounds; // Maximum coordinates of the bounding box
};

struct LifetimeComponent {
    float lifetime;
    std::chrono::steady_clock::time_point spawnTime; // Time when the particle was spawned
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

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glfw";

    window = glfwCreateWindow(1600, 980, "Shooting Game", NULL, NULL);

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

    // Set bounding box for player collider
    float playerSize = 2.0f; // Adjust as needed
    player->collider->minBounds = player->transform->position - glm::vec3(playerSize);
    player->collider->maxBounds = player->transform->position + glm::vec3(playerSize);

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
    player->transform->position = camera->getPosition(); // Update player's position to match camera

    // Debugging output
    //std::cout << "Player position: " << player->transform->position.x << ", " << player->transform->position.y << ", " << player->transform->position.z << std::endl;
    //std::cout << "Camera position: " << camera->getPosition().x << ", " << camera->getPosition().y << ", " << camera->getPosition().z << std::endl;

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

    // Update particle lifetimes and remove expired particles
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
                it = entities.erase(it); // Remove expired particle
                continue; // Skip to the next particle
            }
        }
        ++it;
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

    // Render hit zone around the player
    glm::vec3 minBounds = player->collider->minBounds + player->transform->position;
    glm::vec3 maxBounds = player->collider->maxBounds + player->transform->position;

    std::vector<Vertex> boxVertices;

    // Draw the box by connecting vertices
    // Bottom face
    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    // Top face
    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    // Connect top and bottom faces
    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, minBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    boxVertices.push_back(Vertex::PC(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    tigl::drawVertices(GL_LINES, boxVertices);

    // Render entities
    renderEntities();

    glDisable(GL_DEPTH_TEST);
}



void spawnParticles(float deltaTime)
{
    // Spawn a particle every 2 seconds
    static float spawnTimer = 0.0f;
    spawnTimer += deltaTime;
    if (spawnTimer >= 1.0f) // Adjust spawn rate as needed
    {
        float x = distribution(generator);
        float y = y_distribution(generator);
        float z = distribution(generator);

        auto particle = std::make_unique<Entity>();
        particle->transform->position = glm::vec3(x, y, z);
        particle->transform->scale = glm::vec3(0.5f);

        // Generate random direction components
        float randomX = distribution(generator);
        float randomY = distribution(generator);
        float randomZ = distribution(generator);

        glm::vec3 direction = glm::normalize(glm::vec3(randomX, randomY, randomZ) - particle->transform->position);

        particle->velocity = std::make_unique<VelocityComponent>();
        particle->velocity->velocity = direction * 2.0f; // Adjust speed as needed

        particle->renderable = std::make_unique<RenderableComponent>();
        particle->renderable->vertices = Util::buildCube(particle->transform->position, particle->transform->scale, glm::vec4(1, 0, 0, 1));

        particle->collider = std::make_unique<ColliderComponent>();

        // Set bounding box for particle collider
        float particleSize = 1.0f; // Adjust as needed
        particle->collider->minBounds = particle->transform->position - glm::vec3(particleSize);
        particle->collider->maxBounds = particle->transform->position + glm::vec3(particleSize);

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

bool checkCollision(const Entity& a, const Entity& b) {
    // Check if both entities have colliders
    if (!a.collider || !b.collider)
        return false;

    // Check for overlap along each axis
    bool overlapX = (a.collider->minBounds.x <= b.collider->maxBounds.x && a.collider->maxBounds.x >= b.collider->minBounds.x);
    bool overlapY = (a.collider->minBounds.y <= b.collider->maxBounds.y && a.collider->maxBounds.y >= b.collider->minBounds.y);
    bool overlapZ = (a.collider->minBounds.z <= b.collider->maxBounds.z && a.collider->maxBounds.z >= b.collider->minBounds.z);

    // If there's overlap along all axes, then there's a collision
    return overlapX && overlapY && overlapZ;
}

