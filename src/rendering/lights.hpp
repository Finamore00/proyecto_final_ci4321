#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include "../scene_graph/transform.hpp"

class Light: public SceneObject
{
public:
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;

    void disable_light() {intensity = 0.0f;};
    void enable_light() {intensity = 1.0f;};
};