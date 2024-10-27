#pragma once
#include "../scene_graph/transform.hpp"
#include "../gl_utils/shader.h"
#include "bullet.hpp"

class Tank: public SceneObject {
public:
    Tank(SceneObject &parent, gl_utils::shader_program &shader);
    void update(float time);

private:
    Transform *turret_transform;
    void fire_bullet();
    void rotate_turret(float time);
    void rotate_tank(float time);
    void move(float time);
    Bullet *bullets[3];
};