#include "texture.hpp"

#include <iostream>

#include "../../thirdparty/stb/stb_image.h"

Texture::Texture(const std::vector<std::string> paths, GLenum texture_type, GLint int_format, GLenum format, GLenum data_type): m_textureType(texture_type)
{
    glGenTextures(1, &m_ID);
    glBindTexture(texture_type, m_ID);

    // Texture parameters
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    switch (texture_type)
    {
    case GL_TEXTURE_2D:
        load_2d_text(paths.front(), int_format, format, data_type);
        break;
    case GL_TEXTURE_CUBE_MAP:
        load_cubemap(paths, int_format, format, data_type);
    default:
        break;
    }

    glBindTexture(texture_type, 0);
}

Texture::Texture(unsigned int id, GLenum texture_type, const std::vector<TextureMeta> &metas) : m_ID(id), m_textureType(texture_type), m_file_metas(metas)
{};

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

/// @brief Binds the texture for his use
/// @param unit Texture unit to use for
void Texture::use_texture(GLenum unit) const
{
    glActiveTexture(unit);
    glBindTexture(m_textureType, m_ID);
}

/// @brief Loads a 2d texture from a path
/// @param path Filepath of the texture
/// @param int_format Internal format of the texture
/// @param format Format of the texture
/// @param data_type Type of data in the texture
void Texture::load_2d_text(const std::string& path, GLint int_format, GLenum format, GLenum data_type)
{
    m_file_metas.push_back({});
    unsigned char *data = stbi_load(path.c_str(), &m_file_metas.front().width, &m_file_metas.front().height, &m_file_metas.front().nrChannels, 0); 
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, int_format, m_file_metas.front().width, m_file_metas.front().height, 0, format, data_type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture in path: " << path << std::endl;

    m_file_metas.front().int_format = int_format;
    m_file_metas.front().data_type = data_type;
    m_file_metas.front().format = format;
    stbi_image_free(data);
}

/// @brief Loads a cubemap texture from a path
/// @param path Filepath of the textures composing the cubeap
/// @param int_format Internal format of the texture
/// @param format Format of the texture
/// @param data_type Type of data in the texture
void Texture::load_cubemap(const std::vector<std::string> path, GLint int_format, GLenum format, GLenum data_type)
{
    if (path.size() != 6)
        std::cout << "Wrong texture amount for a cubemap. Expected 6 textures." << std::endl;
        
    m_file_metas.assign(6, {.int_format=(int_format), .format=(format), .data_type=(data_type)});
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char *data = stbi_load(path[i].c_str(), &m_file_metas[i].width, &m_file_metas[i].height, &m_file_metas[i].nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_file_metas[i].width, m_file_metas[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << path[i] << std::endl;
            stbi_image_free(data);
        }
    }
}