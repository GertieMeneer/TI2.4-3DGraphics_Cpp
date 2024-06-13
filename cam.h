#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class cam {
public:
    cam(GLFWwindow* window);

    glm::mat4 getMatrix();
    glm::vec3 getPosition();
    void update(GLFWwindow* window, float deltaTime);

private:
    glm::vec3 position;
    glm::vec3 rotation;

    void move(float angle, float fac, float deltaTime);
};
