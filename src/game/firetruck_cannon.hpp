#pragma once

#include "../scene_graph/component.hpp"

class FiretruckCannonComponent: public Component
{
protected:
    Transform& m_cannon_pivot;
    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    FiretruckCannonComponent(SceneObject* sObj, Transform& cannon);
    void rotate_turret(float dir, float dt);
    void rotate_cannon(float dir, float dt);
    void shoot();

    void update(float dt) override;
};