#pragma once

#include "../../thirdparty/assimp/include/assimp/Importer.hpp"

#include "../resource_management/resource_loader.hpp"

#include "model.hpp"

class ModelLoader : public ResourceLoader<Model>
{
public:
    std::shared_ptr<Model> load_resource(std::string path) const; 
};