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
    unsigned int m_ID;
    std::vector<TextureMeta> m_file_metas;
    const GLenum m_textureType;

    void load_2d_text(const std::string& path, GLint int_format, GLenum format, GLenum data_type);
    void load_cubemap(const std::vector<std::string> path, GLint int_format, GLenum format, GLenum data_type);

public:
    unsigned int get_ID() const {return m_ID;}
    GLenum get_texture_type() const {return m_textureType;}
    void use_texture(GLenum unit) const;
    const std::vector<TextureMeta>& get_texture_properties() const {return m_file_metas;}
    Texture(const std::vector<std::string> paths, GLenum texture_type, GLint int_format, GLenum format, GLenum data_type);
};