
#pragma once

#include <vector>

#include "../scene_graph/component.hpp"
#include "../scene_graph/sceneobject.hpp"

class SpeedoMeterComponent: public Component
{
protected:
    float m_idleRot;
    float m_activeRot;
    float m_curVal;
    bool m_active = false;
    Transform& m_rotTarget;

    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    SpeedoMeterComponent(SceneObject* sObj, float idleRot, float activeRot, Transform& target);
    void update(float dt) override;
};