#include "resource_loader.hpp"
#include "../textures/texture.hpp"

class TextureLoader : public ResourceLoader<Texture>
{
public:
    std::shared_ptr<Texture> load_resource(std::string path); 
};