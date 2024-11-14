#pragma once

#include <vector>
#include <memory>

#include "../../../thirdparty/glad/include/glad/glad.h"
#include "../../../thirdparty/glm/glm.hpp"

class Texture;

class FontAtlas
{
private:
    unsigned int m_startChar;
    unsigned int m_cols, m_rows;
    float m_colOffset, m_rowOffset;
    std::shared_ptr<Texture> m_atlas;

public:
    FontAtlas(std::shared_ptr<Texture> atlas, unsigned int startChar, unsigned int cols, unsigned int rows);

    void use_font_texture(GLenum unit) const;
    glm::vec2 get_char_uv(char c) const;
};