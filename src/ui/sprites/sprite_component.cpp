#include "sprite_component.hpp"

#include <vector>

SpriteComponent::SpriteComponent(SceneObject* sObj, const gl_utils::shader_program& shader, std::shared_ptr<Texture> sprite, float size)
    : UIComponent(sObj, shader), m_texture(sprite)
{
    // Setting quad vbo and vao
    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    set_size(glm::vec2(size));
    glBindVertexArray(0);
}

void SpriteComponent::set_size(const glm::vec2& size)
{
    UIComponent::set_size(size);
    glm::vec2 hSize = size / 2.0f;
    float verts[6][4] = {
        -hSize.x, hSize.y, 0.0f, 1.0f,
        -hSize.x, -hSize.y, 0.0f, 0.0f,
        hSize.x, -hSize.y, 1.0f, 0.0f,
        hSize.x, -hSize.y, 1.0f, 0.0f,
        hSize.x, hSize.y, 1.0f, 1.0f,
        -hSize.x, hSize.y, 0.0f, 1.0f,
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), &verts);
}


void SpriteComponent::set_sprite(const std::shared_ptr<Texture>& sprite)
{
    m_texture = sprite;
}


void SpriteComponent::draw(const glm::mat4& uiProjection)
{
    m_shader->use();
    m_texture.get()->use_texture(GL_TEXTURE0);

    m_shader->set_mat4f("projection", uiProjection);
    m_shader->set_mat4f("model", m_sceneObj->transform.get_model_matrix());

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}