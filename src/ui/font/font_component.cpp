#include "font_component.hpp"

#include <iostream>
#include "../../../thirdparty/glm/glm.hpp"

FontComponent::FontComponent(
    SceneObject* sObj, 
    ResourceManager<FontAtlas>& fontMng, 
    const gl_utils::shader_program& shader, 
    const std::string& fontPath, 
    const std::string& text,
    float size)
    : UIComponent(sObj, shader), m_size(size)
{
    std::shared_ptr<FontAtlas> fAtlas = fontMng.load_resource(fontPath);
    if (fAtlas == nullptr)
    {
        std::cout << "[WARNING] Problem loading font atlas at: " << fontPath << std::endl;
        return;
    }

    m_font = fAtlas;

    // Setting char quad vbo and vao
    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    set_text(text);
    std::cout << "Created font component" << std::endl;
}

/// @brief Sets the text to draw
/// @param text text to draw
void FontComponent::set_text(const std::string& text)
{
    m_text = text;
}

/// @brief Draws the text
/// @param uiProjection UI Layer projection
void FontComponent::draw(const glm::mat4& uiProjection)
{
    m_shader->use();
    m_font.get()->use_font_texture(GL_TEXTURE0);
    m_shader->set_mat4f("projection", uiProjection);
    m_shader->set_mat4f("model", m_sceneObj->transform.get_model_matrix());
    glBindVertexArray(m_VAO);

    // Draw a quad for every char
    unsigned int col = 0;
    unsigned int line = 0;
    for (auto cit = m_text.cbegin(); cit != m_text.cend(); cit++, col++)
    {
        glm::vec3 pos = m_sceneObj->transform.get_world_position();
        if (*cit == '\n')
        {
            line += 1;
            col = 0;
            cit++;
        }

        float vertices[6][4] = {
            {col * m_size, m_size - m_size * line, 0.0f, 1.0f},
            {col * m_size, m_size * line, 0.0f, 0.0f},
            {(col + 1) * m_size, m_size * line, 1.0f, 0.0f},
            {col * m_size, m_size - m_size * line, 0.0f, 1.0f},
            {(col + 1) * m_size, m_size * line, 1.0f, 0.0f},
            {(col + 1) * m_size, m_size - m_size * line, 1.0f, 1.0f}
        };

        glm::vec2 offset = m_font.get()->get_char_uv(*cit);

        m_shader->set_vec2f("charOffset", offset);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //std::cout << "[FONT COMPONENT] End draw text" << std::endl;
}