#include "particle.h"

// Default constructor
#include <glm/glm.hpp>


Particle::Particle()
    : m_position(0.0f), m_velocity(0.0f), m_color(0.0f), m_size(0.0f), m_lifespan(0.0f) {}

// Getter methods implementation
glm::vec3 Particle::getPosition() const {
    return m_position;
}

glm::vec3 Particle::getVelocity() const {
    return m_velocity;
}

glm::vec4 Particle::getColor() const {
    return m_color;
}

float Particle::getSize() const {
    return m_size;
}

float Particle::getLifespan() const {
    return m_lifespan;
}

// Setter methods implementation
void Particle::setPosition(const glm::vec3& position) {
    m_position = position;
}

void Particle::setVelocity(const glm::vec3& velocity) {
    m_velocity = velocity;
}

void Particle::setColor(const glm::vec4& color) {
    m_color = color;
}

void Particle::setSize(float size) {
    m_size = size;
}

void Particle::setLifespan(float lifespan) {
    m_lifespan = lifespan;
}

void Particle::update(float deltaTime) {
    // Update position based on velocity
    m_position += m_velocity * deltaTime;

    // Decrease lifespan
    m_lifespan -= deltaTime;
}
