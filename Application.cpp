#include "Application.h"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>
// #include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

extern Application* app;

Application::Application() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_DEPTH_BITS, 24);

    window = glfwCreateWindow(800, 600, "3D Render", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0); 

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shader = new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    renderer = new Renderer();
}

Application::~Application() {
    delete shader;
    delete camera;
    delete renderer;
    glfwDestroyWindow(window);
    glfwTerminate();
}


void Application::PreloadModels() 
{
    loadModel("models/cube.obj");
}

void Application::run() {
    TextureDataBase::Initialise();
    this->PreloadModels();

    while (!glfwWindowShouldClose(window)) {
        Process();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // std::string szTex("test");
        // Application::applyTexture(TextureDataBase::getTexture("mar1"), shader->getID(), "texture1");

        // shader->use();
        applyTextures();

        renderer->render(shader->getID(), model, view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::applyTextures() 
{
    for (int i = 0; i < 8; ++i) {
        std::string textureUni = "textures[" + std::to_string(i) + "]";
        std::string textureName = "mar" + std::to_string(i + 1);
        GLuint textureID = TextureDataBase::getTexture(textureName.c_str());
        Application::applyTexture(textureID, shader->getID(), textureUni, GL_TEXTURE0 + i);
    }
}

float Application::getMonitorRefreshRate(GLFWmonitor* monitor) {
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (mode) {
        return mode->refreshRate;
    }
    return -1;
}

bool Application::Process() {
    static float lastTime = 0.0f;
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    
    elapsedTime += deltaTime;
    lastTime = currentTime;
    
    frameCount++;

    if (elapsedTime >= 1.0f) { 
        fps = frameCount / elapsedTime;
        frameCount = 0;
        elapsedTime = 0.0f;

        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        float refreshRate = getMonitorRefreshRate(primaryMonitor);
        
        fps = std::min(fps, refreshRate);
        // std::cout << "FPS: " << fps << std::endl;
        return false;
    }  

    processInput(deltaTime);
    return true;
}
void Application::processInput(float deltaTime) 
{
    camera->processKeyboardActions(window);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboardMovement(window, GLFW_KEY_W, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboardMovement(window, GLFW_KEY_S, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboardMovement(window, GLFW_KEY_A, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboardMovement(window, GLFW_KEY_D, deltaTime);
}

void Application::applyTexture(GLuint textureID, GLuint shaderProgramID, const std::string& uniformName, GLenum textureUnit) 
{
    static int textureUnitIndex = 0;
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLint uniformLocation = glGetUniformLocation(shaderProgramID, uniformName.c_str());
    glUniform1i(uniformLocation, textureUnit);
    textureUnitIndex++;
}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true; 

    if (app) {
        if (firstMouse) {
            app->camera->lastX = xpos;
            app->camera->lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - app->camera->lastX;
        float yoffset = app->camera->lastY - ypos; 

        app->camera->processMouseMovement(xoffset, yoffset);

        app->camera->lastX = xpos;
        app->camera->lastY = ypos;
    } else {
        std::cerr << "Application pointer is null in mouse_callback" << std::endl;
    }
}

void Application::loadModel(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        std::cerr << "Failed to load model: " << err << std::endl;
        return;
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape : shapes) {
        for (size_t i = 0; i < shape.mesh.indices.size(); ++i) {
            const auto& index = shape.mesh.indices[i];

            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (!attrib.texcoords.empty()) {
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            } else {
                vertices.push_back(0.0f);  
                vertices.push_back(0.0f);
            }

            vertices.push_back(1.0f); // r
            vertices.push_back(1.0f); // g
            vertices.push_back(1.0f); // b

            indices.push_back(static_cast<unsigned int>(i));
        }
    }

    renderer->Begin();
    renderer->setupIndicies(indices.data(), indices.size() * sizeof(unsigned int));
    renderer->setupVertices(vertices.data(), vertices.size() * sizeof(float));
    renderer->apply_setups();
    renderer->End();
}
