#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>        
#include <glm/gtc/type_ptr.hpp> 

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use();
    GLuint getID() const { return shaderID; }

    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    GLuint shaderID;

    GLuint compileShader(GLenum type, const char* source);
    void linkProgram(GLuint vertexShader, GLuint fragmentShader);
    std::string loadShaderSource(const char* filePath);

    void checkCompileErrors(GLuint shader, std::string type);
};

#endif // SHADER_H
