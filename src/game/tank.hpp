#pragma once
#include "../scene_graph/transform.hpp"
#include "../gl_utils/shader.h"

class Tank: public SceneObject {
    public:
     Tank(SceneObject &parent, gl_utils::shader_program &shader);
};