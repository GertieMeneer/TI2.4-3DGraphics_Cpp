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

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
cam* camera;

// System Prototypes
void init();
void update(float deltaTime);
void draw();
void spawnParticles(float deltaTime);
void moveEntities(float deltaTime);
void renderEntities();
bool checkCollision(const Entity& a, const Entity& b);
void drawParticleColliderBoundsBox();
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
    player->transform.position = glm::vec3(0, 0, 0);
    player->collider = std::make_unique<ColliderComponent>();

    // Set bounding box for player collider
    float playerSize = 1.0f; // Adjust as needed
    player->collider->minBounds = glm::vec3(-playerSize / 2.0f);
    player->collider->maxBounds = glm::vec3(playerSize / 2.0f);

    entities.push_back(std::unique_ptr<Entity>(player));

    floorEntity = new Entity();
    floorEntity->transform.position = glm::vec3(0, -1, 0);
    floorEntity->renderable = std::make_unique<RenderableComponent>();
    floorEntity->renderable->vertices = Util::buildFloor();

    entities.push_back(std::unique_ptr<Entity>(floorEntity));
    std::cout << "Player collider bounds along Y-axis: (" << player->collider->minBounds.y << ", " << player->collider->maxBounds.y << ")" << std::endl;
}

void update(float deltaTime)
{
    camera->update(window, deltaTime);
    player->transform.position = camera->getPosition(); // Update player's position to match camera

    // Debugging output
    //std::cout << "Player position: " << player->transform->position.x << ", " << player->transform->position.y << ", " << player->transform->position.z << std::endl;

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
    Util::drawPlayerColliderBoundsBox(player, camera);

    // Render collider boxes around particles
    drawParticleColliderBoundsBox();

    // Render the floor
    renderFloor();

    // Render entities
    renderEntities();

    glDisable(GL_DEPTH_TEST);
}




void drawParticleColliderBoundsBox()
{
    // Loop through all entities and draw collider boxes for particles
    // In drawParticleColliderBoundsBox() function:

// Loop through all entities and draw collider boxes for particles
    for (const auto& entityPtr : entities) {
        const Entity& entity = *entityPtr; // Dereference std::unique_ptr<Entity>
        // Check if the entity is a particle and has a collider
        if (&entity != player && entity.collider) {
            // Get particle's collider bounds
            glm::vec3 minBounds = entity.collider->minBounds;
            glm::vec3 maxBounds = entity.collider->maxBounds;

            // Calculate the corners of the box relative to the particle's position
            glm::vec3 corners[8] = {
                entity.transform.position + glm::vec3(minBounds.x, minBounds.y, minBounds.z),
                entity.transform.position + glm::vec3(maxBounds.x, minBounds.y, minBounds.z),
                entity.transform.position + glm::vec3(maxBounds.x, minBounds.y, maxBounds.z),
                entity.transform.position + glm::vec3(minBounds.x, minBounds.y, maxBounds.z),
                entity.transform.position + glm::vec3(minBounds.x, maxBounds.y, minBounds.z),
                entity.transform.position + glm::vec3(maxBounds.x, maxBounds.y, minBounds.z),
                entity.transform.position + glm::vec3(maxBounds.x, maxBounds.y, maxBounds.z),
                entity.transform.position + glm::vec3(minBounds.x, maxBounds.y, maxBounds.z)
            };

            // Apply camera's view matrix
            glm::mat4 viewMatrix = camera->getMatrix();

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
        particle->transform.position = glm::vec3(x, 7.0f, z); // Start position at a height
        particle->transform.scale = glm::vec3(0.5f);

        // Set direction to move downwards
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

        particle->velocity = std::make_unique<VelocityComponent>();
        particle->velocity->velocity = direction * 10.0f; // Adjust speed as needed

        particle->renderable = std::make_unique<RenderableComponent>();
        particle->renderable->vertices = Util::buildCube(particle->transform.position, particle->transform.scale, glm::vec4(1, 0, 0, 1));

        particle->collider = std::make_unique<ColliderComponent>();

        // Set bounding box for particle collider
        float particleSize = 1.0f; // Adjust as needed
        particle->collider->minBounds = particle->transform.position - glm::vec3(particleSize);
        particle->collider->maxBounds = particle->transform.position + glm::vec3(particleSize);

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

