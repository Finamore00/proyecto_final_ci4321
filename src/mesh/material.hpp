#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include "../textures/texture.hpp"

/// @brief Struct representing the material properties of a shader
struct Material
{
    Texture* texture = nullptr;
    glm::vec2 texUVoffset = glm::vec2(0.0f);
    glm::vec2 texUVscales = glm::vec2(1.0f);
    glm::vec3 ambient = glm::vec3(0.1f);
    glm::vec3 tint = glm::vec3(1.0f);
};