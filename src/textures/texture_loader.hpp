#pragma once
#include "../resource_management/resource_loader.hpp"
#include "../textures/texture.hpp"

/// @brief Loads textures
class TextureLoader : public ResourceLoader<Texture>
{
public:
    std::shared_ptr<Texture> load_resource(std::string path) const; 
};