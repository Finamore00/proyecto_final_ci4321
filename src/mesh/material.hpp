#pragma once

#include <memory>

#include "../../thirdparty/glm/glm.hpp"
#include "../textures/texture.hpp"

using std::shared_ptr;

/// @brief Struct representing the material properties of a shader
struct Material
{
    shared_ptr<Texture> albedo = nullptr;
    glm::vec2 texUVoffset = glm::vec2(0.0f);
    glm::vec2 texUVscales = glm::vec2(1.0f);
    glm::vec3 ambient = glm::vec3(0.05f);
    glm::vec3 tint = glm::vec3(0.0f);
};