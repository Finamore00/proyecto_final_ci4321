#pragma once

#include <vector>

#include "../../scene_graph/sceneobject.hpp"
#include "../../ui/font/font_component.hpp"

class ObstacleCounterComponent: public Component
{
protected:
    unsigned int m_count = 0;
    FontComponent* m_text;
    std::vector<SceneObject*> m_obstacles;

    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    ObstacleCounterComponent(SceneObject* sObj, const std::vector<SceneObject*>& obstacles, FontComponent* text);
    void update(float dt) override;
};