#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include "../scene_graph/sceneobject.hpp"
#include "light_source_type.hpp"



class Light: public SceneObject
{
public:
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
    LightSourceType type = LightSourceType::Point;
    float cutoff = INFINITY;
    float intensity = 1.0f;

    void disable_light() { intensity = 0.0f; };
    void enable_light() { intensity = 1.0f; };
};