#pragma once

#include "../scene_graph/component.hpp"
#include "../gl_utils/shader.h"

class UIComponent : public Component
{
protected:
    const gl_utils::shader_program* m_shader;
    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    UIComponent(SceneObject* sObj, const gl_utils::shader_program& shader): Component(sObj), m_shader(&shader) {}
    virtual void draw(const glm::mat4& uiProjection) = 0;
};