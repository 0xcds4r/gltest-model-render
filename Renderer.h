#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Begin();
    void End();
    void setupIndicies(const void* data, size_t size);
    void setupVertices(const void* data, size_t size);
    void apply_setups();
    void render(GLuint shaderID, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
    void* indices;
    size_t indicesSize;
    void* vertices;
    size_t vertexSize;
    GLuint VAO, VBO, EBO;
};

#endif // RENDERER_H
