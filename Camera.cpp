#include "Camera.h"
#include <cmath>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(8.25f), mouseSensitivity(0.08f), yaw(yaw), pitch(pitch), worldUp(up) {
    this->position = position;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboardMovement(GLFWwindow* window, int key, float deltaTime) 
{
    float velocity = movementSpeed * deltaTime;
    if (key == GLFW_KEY_W)
        position += front * velocity;
    if (key == GLFW_KEY_S)
        position -= front * velocity;
    if (key == GLFW_KEY_A)
        position -= right * velocity;
    if (key == GLFW_KEY_D)
        position += right * velocity;
}

void Camera::processKeyboardActions(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        movementSpeed += 0.01f;
        std::cout << "Movement Speed increased to: " << movementSpeed << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        movementSpeed -= 0.01f;
        if (movementSpeed < 0.1f) movementSpeed = 0.1f;  // Ќе позвол€йте скорости быть слишком маленькой
        std::cout << "Movement Speed decreased to: " << movementSpeed << std::endl;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;  
    pitch += yoffset; 

    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    // std::cout << "Front: " << this->front.x << ", " << this->front.y << ", " << this->front.z << std::endl;

    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));

    // std::cout << "Right: " << this->right.x << ", " << this->right.y << ", " << this->right.z << std::endl;
    // std::cout << "Up: " << this->up.x << ", " << this->up.y << ", " << this->up.z << std::endl;
}