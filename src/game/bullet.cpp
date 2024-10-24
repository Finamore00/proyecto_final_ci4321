#include "bullet.hpp"

Bullet::Bullet(float speed, float gravityAccel, bool useGravity) 
    : m_speed(speed), m_gravityAccel(gravityAccel), m_useGravity(useGravity)
{}

/// @brief Spawns a bullet using its own transform as direction
void Bullet::spawn()
{
    m_velocity = transform.get_front_vector() * m_speed;
}

/// @brief Spawns a bullet using another transform as direction/position
/// @param spawner Bullet spawner
void Bullet::spawn(const Transform& spawner)
{
    transform.set_world_position(spawner.get_world_position());
    transform.set_world_rotation(spawner.get_world_rotation());
    m_velocity = spawner.get_front_vector() * m_speed;
}

void Bullet::update(float dt)
{
    auto wp = transform.get_world_position();
    auto next = wp + m_velocity * dt;
    auto dtvel = m_velocity * dt;

    if (m_useGravity)
        m_velocity += glm::vec3(0.0f, 1.0f, 0.0f) * m_gravityAccel * dt;


    transform.set_world_position(transform.get_world_position() + m_velocity * dt);
}

void Bullet::on_collision(Collider& collider, Transform& transform)
{
    enabled = false;
}