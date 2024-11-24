#pragma once

#include "../scene_graph/component.hpp"
#include "../gl_utils/shader.hpp"

class UIComponent : public Component
{
protected:
    glm::vec2 m_size = glm::vec2(1.0f);
    const gl_utils::shader_program* m_shader;
    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    UIComponent(SceneObject* sObj, const gl_utils::shader_program& shader): Component(sObj), m_shader(&shader) {}

    virtual void set_size(const glm::vec2& size);
    virtual void draw(const glm::mat4& uiProjection) = 0;
};