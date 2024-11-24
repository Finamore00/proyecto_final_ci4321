#pragma once

#include <memory>

#include "../../textures/texture.hpp"
#include "../../gl_utils/shader.hpp"
#include "../ui_component.hpp"

class SpriteComponent: public UIComponent
{
protected:
    unsigned int m_VAO, m_VBO;
    std::shared_ptr<Texture> m_texture;

    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    SpriteComponent(SceneObject* sObj, const gl_utils::shader_program& shader, std::shared_ptr<Texture> sprite, float size);
    void set_size(const glm::vec2& size) override;
    void set_sprite(const std::shared_ptr<Texture>& sprite);
    void draw(const glm::mat4& uiProjection) override;
};