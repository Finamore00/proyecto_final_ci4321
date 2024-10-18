#pragma once
#include <glad/glad.h>

struct TextureMeta
{
    int width, height, nrChannels; 
    GLint int_format;
    GLenum format, data_type;
};

class Texture
{
private:
    unsigned int _ID;
    TextureMeta _file_meta;

public:
    void useTexture(GLenum unit);
    const TextureMeta& getTextureProperties();
    Texture(const char* path, GLint int_format, GLenum format, GLenum data_type);
};