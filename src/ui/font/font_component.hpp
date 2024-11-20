#pragma once

#include "font_atlas.hpp"

#include "../../resource_management/resource_manager.hpp"
#include "../ui_component.hpp"


class FontComponent : public UIComponent
{
private:
    unsigned int m_VAO, m_VBO;
    float m_size = 32.0f;

    std::string m_text;
    std::shared_ptr<FontAtlas> m_font;

protected:
    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    FontComponent(
        SceneObject* sObj, 
        ResourceManager<FontAtlas>& fontMng, 
        const gl_utils::shader_program& shader, 
        const std::string& fontPath, 
        const std::string& text,
        float size);

    void set_text(const std::string& text);
    void draw(const glm::mat4& uiProjection) override;
};