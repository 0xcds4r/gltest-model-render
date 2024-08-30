#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    glm::mat4 getViewMatrix() const;
    void processKeyboardMovement(GLFWwindow* window, int key, float deltaTime);
    void processKeyboardActions(GLFWwindow* window);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }

private:
    void updateCameraVectors(); 

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

public:
    float lastX = 0.0f;
    float lastY = 0.0f;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float mouseSensitivity = 0.1f;
    float movementSpeed;
};

#endif // CAMERA_H
