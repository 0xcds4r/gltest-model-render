#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include "TextureDatabase.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"

class Application {
public:
    Application();
    ~Application();
    void run();
    void loadModel(const std::string& path);
    // GLuint loadTexture(const std::string& filename);

    static void applyTexture(GLuint textureID, GLuint shaderProgramID, const std::string& uniformName, GLenum textureUnit = GL_TEXTURE0);

    GLFWwindow* GetWindow() {
        return this->window;
    };

    void applyTextures();

    void calculateFPS();
    float getMonitorRefreshRate(GLFWmonitor* monitor);
    void PreloadModels();
private:
    bool Process();
    void processInput(float deltaTime);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    GLFWwindow* window;
    Shader* shader;
    Renderer* renderer;
    Camera* camera;

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    unsigned int frameCount = 0;
    float elapsedTime = 0.0f;
    float fps = 0.0f;
};

#endif // APPLICATION_H
