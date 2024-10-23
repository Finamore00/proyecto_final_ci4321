#include "bullet.hpp"

Bullet::Bullet(float speed, float gravityAccel, bool useGravity) 
    : m_speed(speed), m_gravityAccel(gravityAccel), m_useGravity(useGravity)
{}

void Bullet::spawn()
{
    m_velocity = transform.getFrontVector() * m_speed;
}

void Bullet::spawn(const Transform& spawner)
{
    transform.setWorldPosition(spawner.getWorldPosition());
    transform.setWorldRotation(spawner.getWorldRotation());
    m_velocity = spawner.getFrontVector() * m_speed;
}

void Bullet::update(float dt)
{
    if (m_useGravity)
        m_velocity += glm::vec3(0.0f, 1.0f, 0.0f) * m_gravityAccel * dt;

    transform.setWorldPosition(transform.getWorldPosition() + m_velocity * dt);
}

void Bullet::OnCollision(Collider& collider, Transform& transform)
{
    enabled = false;
}