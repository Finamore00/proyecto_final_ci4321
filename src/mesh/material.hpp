#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include "../textures/texture.hpp"

struct Material
{
    Texture* texture = nullptr;
    glm::vec3 color = glm::vec3(1.0f);
};