#pragma once
#include "../scene_graph/transform.hpp"
#include "../gl_utils/shader.h"
#include "bullet.hpp"

class Tank: public SceneObject {
public:
    Tank(SceneObject &parent, gl_utils::shader_program &shader);
    void update(float time);
    void update_bullets(float time);
    Bullet *bullets[3];

   private:
    Transform *turret_transform;
    Transform *spawner_transform;
    Transform *cannon_pivot_transform;
    void fire_bullet();
    void rotate_turret(float time);
    void rotate_tank(float time);
    void move(float time);
    float last_fired_time = 0.0f;
};