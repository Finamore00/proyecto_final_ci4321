#pragma once

#include "light_source_type.hpp"

#include "../../thirdparty/glm/glm.hpp"
#include "../scene_graph/component.hpp"

class LightComponent: public Component
{
protected:
    LightComponent(SceneObject* sObj);\
    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

    LightSourceType m_type = LightSourceType::Directional;

public:
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;

    void update(float dt) override {};

    LightSourceType get_type() const {return m_type;}
    void set_type(LightSourceType type);
};