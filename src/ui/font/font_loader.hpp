#pragma once
#include "font_atlas.hpp"

#include "../resource_management/resource_loader.hpp"
#include "../resource_management/resource_manager.hpp"

class Texture;

class FontLoader : public ResourceLoader<FontAtlas>
{
private:
    ResourceManager<Texture>& m_txLoader;

public:
    std::shared_ptr<FontAtlas> load_resource(std::string path) const; 
    FontLoader(ResourceManager<Texture>& txLoader);
};