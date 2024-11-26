#pragma once

#include <vector>
#include <memory>

#include "mesh.hpp"

/// @brief Class that represents a model as a collection of meshes
class Model 
{
private:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
public:
    unsigned int mesh_number() const;
    Mesh* get_mesh(unsigned int m) const; // temporal, check if i should really be using unique_ptrs

    void add_mesh(std::unique_ptr<Mesh> mesh);
    void draw(const glm::mat4& model) const;
};