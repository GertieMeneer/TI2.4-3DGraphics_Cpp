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
void drawPlayerColliderBoundsBox();
void renderFloor();

// Globals
std::vector<std::unique_ptr<Entity>> entities;
Entity* player;
Entity* floorEntity;

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-5.0f, 5.0f);
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
    float playerSize = 1.0f; // Adjust as needed
    player->collider->minBounds = player->transform->position - glm::vec3(playerSize);
    player->collider->maxBounds = player->transform->position + glm::vec3(playerSize);

    entities.push_back(std::unique_ptr<Entity>(player));

    floorEntity = new Entity();
    floorEntity->transform->position = glm::vec3(0, -1, 0);
    floorEntity->renderable = std::make_unique<RenderableComponent>();
    floorEntity->renderable->vertices = Util::buildFloor();

    entities.push_back(std::unique_ptr<Entity>(floorEntity));
}

void update(float deltaTime)
{
    camera->update(window, deltaTime);
    player->transform->position = camera->getPosition(); // Update player's position to match camera

    // Debugging output
    std::cout << "Player position: " << player->transform->position.x << ", " << player->transform->position.y << ", " << player->transform->position.z << std::endl;

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
    glm::mat4 projection = glm::perspective(glm::radians(100.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    tigl::shader->enableColor(true);

    glEnable(GL_DEPTH_TEST);

    // Render hit zone around the player
    drawPlayerColliderBoundsBox();

    // Render the floor
    renderFloor();

    // Render entities
    renderEntities();

    glDisable(GL_DEPTH_TEST);
}

void drawPlayerColliderBoundsBox()
{
    // Get player's collider bounds
    glm::vec3 minBounds = player->collider->minBounds + player->transform->position;
    glm::vec3 maxBounds = player->collider->maxBounds + player->transform->position;

    // Calculate the corners of the box
    std::vector<glm::vec3> corners;
    corners.push_back(glm::vec3(minBounds.x, minBounds.y, minBounds.z));
    corners.push_back(glm::vec3(maxBounds.x, minBounds.y, minBounds.z));
    corners.push_back(glm::vec3(minBounds.x, maxBounds.y, minBounds.z));
    corners.push_back(glm::vec3(maxBounds.x, maxBounds.y, minBounds.z));
    corners.push_back(glm::vec3(minBounds.x, minBounds.y, maxBounds.z));
    corners.push_back(glm::vec3(maxBounds.x, minBounds.y, maxBounds.z));
    corners.push_back(glm::vec3(minBounds.x, maxBounds.y, maxBounds.z));
    corners.push_back(glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z));

    // Apply camera's view matrix
    glm::mat4 viewMatrix = camera->getMatrix();

    // Draw the box using GL_LINES
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white

    for (const auto& corner : corners)
    {
        glm::vec4 transformedCorner = viewMatrix * glm::vec4(corner, 1.0f);
        glVertex3f(transformedCorner.x, transformedCorner.y, transformedCorner.z);
    }

    // Bottom face
    glVertex3f(corners[0].x, corners[0].y, corners[0].z);
    glVertex3f(corners[1].x, corners[1].y, corners[1].z);

    glVertex3f(corners[1].x, corners[1].y, corners[1].z);
    glVertex3f(corners[3].x, corners[3].y, corners[3].z);

    glVertex3f(corners[3].x, corners[3].y, corners[3].z);
    glVertex3f(corners[2].x, corners[2].y, corners[2].z);

    glVertex3f(corners[2].x, corners[2].y, corners[2].z);
    glVertex3f(corners[0].x, corners[0].y, corners[0].z);

    // Top face
    glVertex3f(corners[4].x, corners[4].y, corners[4].z);
    glVertex3f(corners[5].x, corners[5].y, corners[5].z);

    glVertex3f(corners[5].x, corners[5].y, corners[5].z);
    glVertex3f(corners[7].x, corners[7].y, corners[7].z);

    glVertex3f(corners[7].x, corners[7].y, corners[7].z);
    glVertex3f(corners[6].x, corners[6].y, corners[6].z);

    glVertex3f(corners[6].x, corners[6].y, corners[6].z);
    glVertex3f(corners[4].x, corners[4].y, corners[4].z);

    // Connections between top and bottom faces
    glVertex3f(corners[0].x, corners[0].y, corners[0].z);
    glVertex3f(corners[4].x, corners[4].y, corners[4].z);

    glVertex3f(corners[1].x, corners[1].y, corners[1].z);
    glVertex3f(corners[5].x, corners[5].y, corners[5].z);

    glVertex3f(corners[2].x, corners[2].y, corners[2].z);
    glVertex3f(corners[6].x, corners[6].y, corners[6].z);

    glVertex3f(corners[3].x, corners[3].y, corners[3].z);
    glVertex3f(corners[7].x, corners[7].y, corners[7].z);
    glEnd();
}

void renderFloor()
{
    if (floorEntity->renderable)
    {
        tigl::drawVertices(GL_QUADS, floorEntity->renderable->vertices);
    }
}

void spawnParticles(float deltaTime)
{
    // Spawn a particle every 2 seconds
    static float spawnTimer = 0.0f;
    spawnTimer += deltaTime;
    if (spawnTimer >= 0.5f) // Adjust spawn rate as needed
    {
        float x = distribution(generator);
        float z = distribution(generator);

        auto particle = std::make_unique<Entity>();
        particle->transform->position = glm::vec3(x, 7.0f, z); // Start position at a height
        particle->transform->scale = glm::vec3(0.5f);

        // Set direction to move downwards
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

        particle->velocity = std::make_unique<VelocityComponent>();
        particle->velocity->velocity = direction * 10.0f; // Adjust speed as needed

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
        if (entity->renderable && entity.get() != floorEntity) // Skip floor rendering here
        {
            tigl::drawVertices(GL_QUADS, entity->renderable->vertices);
        }
    }
}

bool checkCollision(const Entity& a, const Entity& b)
{
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

