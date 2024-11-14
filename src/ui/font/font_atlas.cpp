#include "font_atlas.hpp"

#include <iostream>

#include "../../textures/texture.hpp"

FontAtlas::FontAtlas(std::shared_ptr<Texture> atlas, unsigned int startChar, unsigned int cols, unsigned int rows)
    : m_atlas(atlas), m_startChar(startChar), m_cols(cols), m_rows(rows)
{
    const TextureMeta txMeta = *atlas.get()->get_texture_properties().begin();
    unsigned int w = txMeta.width;
    unsigned int h = txMeta.height;

    m_colOffset = 1.0f / cols;
    m_rowOffset = 1.0f / rows;
}

void FontAtlas::use_font_texture(GLenum unit) const
{
    m_atlas.get()->use_texture(unit);
}

glm::vec2 FontAtlas::get_char_uv(char c) const
{
    unsigned int ascii = (unsigned int)c;
    unsigned int pos = ascii - m_startChar;
    if (pos < 0 || pos >= m_cols * m_rows)
    {
        std::cout << "[WARNING] Trying to access char not in atlas: " << c << std::endl;
        return glm::vec2(0, 0);
    }

    float row = floor((float)pos / m_cols);
    float col = pos % m_cols;

    //std::cout << "[FONT ATLAS] Char \"" << c << "\" col: " << col << " row: " << row << std::endl;
    return glm::vec2(m_colOffset * col, -m_rowOffset * (row + 1));
}