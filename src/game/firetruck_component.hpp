#pragma once

#include "../scene_graph/component.hpp"

class FiretruckComponent: public Component
{
protected:
    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    FiretruckComponent(SceneObject* sObj);
    void rotate_tank(float dir, float dt);
    void move(const glm::vec3& dir, float dt);

    void update(float dt) override;
};