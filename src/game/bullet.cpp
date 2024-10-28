#include "bullet.hpp"
#include <iostream>

Bullet::Bullet(float xSpeed, float gravityAccel, bool useGravity) 
    : m_xSpeed(xSpeed), m_gravityAccel(gravityAccel), m_useGravity(useGravity)
{}

/// @brief Spawns a bullet using its own transform as direction
void Bullet::spawn(float initialSpeed)
{
    glm::vec3 f = transform.get_front_vector();
    m_velocity = f * initialSpeed;
    m_horVector = glm::normalize(glm::vec3(f.x, 0.0f, f.z));
}

/// @brief Spawns a bullet using another transform as direction/position
/// @param spawner Bullet spawner
void Bullet::spawn(const Transform& spawner, float initialSpeed, bool useGravity)
{
    transform.set_world_position(spawner.get_world_position());
    transform.set_world_rotation(spawner.get_world_rotation());

    m_useGravity = useGravity;

    glm::vec3 f = spawner.get_front_vector();
    m_velocity = f * initialSpeed;
    m_horVector = glm::normalize(glm::vec3(f.x, 0.0f, f.z));
}

void Bullet::update(float dt)
{
    if (m_useGravity)
        m_velocity += glm::vec3(0.0f, 1.0f, 0.0f) * m_gravityAccel * dt;

    glm::vec3 fVel = (m_velocity + m_horVector) * dt;
    transform.set_world_position(transform.get_world_position() + fVel);
}

void Bullet::on_collision(Collider& collider, Transform& transform)
{
    enabled = false;
}