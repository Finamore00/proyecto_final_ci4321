#pragma once

#include <vector>

#include "../../scene_graph/sceneobject.hpp"
#include "../../ui/font/font_component.hpp"

class ObstacleCounterComponent: public Component
{
protected:
    unsigned int m_count = 0;
    std::weak_ptr<FontComponent> m_text;
    std::vector<SceneObject*> m_obstacles;

    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    ObstacleCounterComponent(SceneObject* sObj, const std::vector<SceneObject*>& obstacles, std::weak_ptr<FontComponent> text);
    void update(float dt) override;
};