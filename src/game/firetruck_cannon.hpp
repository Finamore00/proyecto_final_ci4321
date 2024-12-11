#pragma once

#include <vector>

#include "bullet_component.hpp"

#include "../scene_graph/component.hpp"
#include "../particles/particle_emitter.hpp"

class FiretruckCannonComponent: public Component
{
protected:
    Transform& m_cannon_pivot;
    Transform& m_bullet_spawn;
    std::vector<std::weak_ptr<BulletComponent>> m_bullets;
    std::weak_ptr<ParticleEmitter> m_vfx;

    unsigned int m_last_bullet = 0;
    float m_fire_rate = 0.2f;
    float m_last_shot_time = 0.0f;
    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    FiretruckCannonComponent(SceneObject *sObj, Transform &cannon, Transform& bulletSpawn, const std::vector<SceneObject*>& bullets, std::weak_ptr<ParticleEmitter> vfx);
    void rotate_turret(float dir, float dt);
    void rotate_cannon(float dir, float dt);
    void shoot(bool grav);

    void update(float dt) override;
};