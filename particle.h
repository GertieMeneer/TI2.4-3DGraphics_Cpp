#include <glm/glm.hpp>


class Particle {
public:
    // Default constructor
    Particle();

    // Getter methods
    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const;
    glm::vec4 getColor() const;
    float getSize() const;
    float getLifespan() const;

    // Setter methods
    void setPosition(const glm::vec3& position);
    void setVelocity(const glm::vec3& velocity);
    void setColor(const glm::vec4& color);
    void setSize(float size);
    void setLifespan(float lifespan);

    // Update method
    void update(float deltaTime);

private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec4 m_color;
    float m_size;
    float m_lifespan;
};
