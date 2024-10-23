#pragma once
#include "../scene_graph/transform.hpp"

class Bullet: public SceneObject
{
private:
    float m_speed = 10.0f;
    float m_gravityAccel = 9.8f;
    bool m_useGravity;
    glm::vec3 m_velocity = glm::vec3(0.0f);

public:
    Bullet(float speed, float gravityAccel, bool useGravity);
    void spawn();
    void spawn(const Transform& spawner);
    void update(float dt);
    void OnCollision(Collider& collider, Transform& transform) override;
};