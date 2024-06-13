#include "cam.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

cam::cam(GLFWwindow* window) : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

glm::mat4 cam::getMatrix()
{
    glm::mat4 ret(1.0f);        //identity matrix
    ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));     //make rotation matrix around x-axis
    ret = glm::rotate(ret, rotation.y, glm::vec3(0, 1, 0));     //make rotation matrix around y-axis
    ret = glm::translate(ret, -position);       //move opposite direction: scene moves in opposite direction of cam
    return ret;
}

glm::vec3 cam::getPosition()
{
    return position;
}

void cam::move(float angle, float fac, float deltaTime)
{
    float velocity = fac * deltaTime;
    //update cam pos in direction based on angle of movement
    position.x += cos(rotation.y + glm::radians(angle)) * velocity;
    position.z += sin(rotation.y + glm::radians(angle)) * velocity;
}

void cam::update(GLFWwindow* window, float deltaTime)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);       //get and store cursor position

    static double lastX = x;
    static double lastY = y;

    //calculate change in cursor position since last frame
    rotation.x -= static_cast<float>(lastY - y) / 750.0f;
    rotation.y -= static_cast<float>(lastX - x) / 750.0f;

    //store current cursor pos for next frame
    lastX = x;
    lastY = y;

    //keyboard input
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        move(180, 10.0f, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        move(0, 10.0f, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        move(-90, 10.0f, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        move(90, 10.0f, deltaTime);

    /*if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position.y += 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        position.y -= 10.0f * deltaTime;*/
}
