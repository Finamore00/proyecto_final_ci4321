#include "texture.hpp"
#include "../../thirdparty/stb/stb_image.h"

#include <glad/glad.h>
#include <iostream>

Texture::Texture(const char* path, GLint int_format, GLenum format, GLenum data_type)
{
    glGenTextures(1, &_ID);
    glBindTexture(GL_TEXTURE_2D, _ID);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    unsigned char *data = stbi_load(path, &_file_meta.width, &_file_meta.height, &_file_meta.nrChannels, 0); 
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, int_format, _file_meta.width, _file_meta.height, 0, format, data_type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture in path: " << path << std::endl;
        // Arrojar error o algo para poder handlearlo just in case
        // deletear textura y eso. O un magenta texture :think:
    }

    _file_meta.int_format = int_format;
    _file_meta.data_type = data_type;
    _file_meta.format = format;
    stbi_image_free(data);
}

void Texture::useTexture(GLenum unit)
{
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, _ID);
}

const TextureMeta& Texture::getTextureProperties()
{
    return _file_meta;
}