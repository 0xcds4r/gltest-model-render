#include "Renderer.h"
#include <iostream>

Renderer::Renderer() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Renderer::Begin() {
    glBindVertexArray(VAO);
}

void Renderer::End() {
    glBindVertexArray(0);
}

void Renderer::apply_setups() 
{
    // Позиции
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Координаты текстур
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    // Цвета
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::setupIndicies(const void* data, size_t size) {
    indicesSize = size;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Renderer::setupVertices(const void* data, size_t size) {
    vertexSize = size;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Renderer::render(GLuint shaderID, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) 
{
    GLuint modelLoc = glGetUniformLocation(shaderID, "model");
    GLuint viewLoc = glGetUniformLocation(shaderID, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderID, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, indicesSize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
