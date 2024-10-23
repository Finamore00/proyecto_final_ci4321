#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>

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
    std::vector<TextureMeta> _file_metas;
    const GLenum m_textureType;

    void load_2d_text(const std::string& path, GLint int_format, GLenum format, GLenum data_type);
    void load_cubemap(const std::vector<std::string> path, GLint int_format, GLenum format, GLenum data_type);

public:
    unsigned int getId() const {return _ID;}
    void useTexture(GLenum unit) const;
    const std::vector<TextureMeta>& getTextureProperties() const {return _file_metas;}
    Texture(const std::vector<std::string> paths, GLenum texture_type, GLint int_format, GLenum format, GLenum data_type);
};