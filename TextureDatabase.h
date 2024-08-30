#ifndef TEXTURE_DATABASE_H
#define TEXTURE_DATABASE_H

#include <list>
#include <GL/glew.h>

struct TextureData {
    const char* name;
    const char* path;
    GLuint textureID;
    TextureData(const char* n, const char* p, GLuint id) : name(n), path(p), textureID(id) {}
};

class TextureDataBase {
public:
    static void Initialise();
    static GLuint retrieveOrLoadTexture(const char* name, const char* path);
    static GLuint loadTexture(const char* filename);
    static GLuint getTexture(const char* name);

private:
    static std::list<TextureData> textureCache;
};

#endif // TEXTURE_DATABASE_H
