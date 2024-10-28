#pragma once
#include "../scene_graph/transform.hpp"

class Bullet: public SceneObject
{
private:
    const float m_xSpeed = 10.0f;
    const float m_gravityAccel = 9.8f;
    bool m_useGravity;
    glm::vec3 m_velocity = glm::vec3(0.0f);
    glm::vec3 m_horVector;

public:
    Bullet(float speed, float gravityAccel, bool useGravity);
    void spawn(float initialSpeed);
    void spawn(const Transform& spawner, float initialSpeed, bool useGravity);
    void update(float dt);
    void on_collision(Collider& collider, Transform& transform) override;
};