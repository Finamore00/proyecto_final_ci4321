#pragma once

#include "../scene_graph/component.hpp"

class BulletComponent: public Component
{
private:
    const float m_xSpeed = 10.0f;
    const float m_gravityAccel = 9.8f;
    bool m_useGravity;
    glm::vec3 m_velocity = glm::vec3(0.0f);
    glm::vec3 m_horVector;

protected:
    void __on_collision(ColliderComponent& collider, Transform& transform) override;

public:
    BulletComponent(SceneObject* sObj, float speed, float gravityAccel, bool useGravity);
    void spawn(float initialSpeed);
    void spawn(const Transform& spawner, float initialSpeed, bool useGravity);
    void update(float dt) override;
};