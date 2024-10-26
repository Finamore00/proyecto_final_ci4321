#pragma once
#include "../scene_graph/transform.hpp"
#include "../gl_utils/shader.h"
#include "bullet.hpp"

class Tank: public SceneObject {
    public:
        Tank(SceneObject &parent, gl_utils::shader_program &shader);
        void rotate_turret(glm::vec3 euler_rotations, float time);
        void fire_bullet();

   private:
        Bullet *bullets[3];
};