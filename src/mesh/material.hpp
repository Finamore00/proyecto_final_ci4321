#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include "../textures/texture.hpp"

struct Material
{
    Texture* texture = nullptr;
    glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
};