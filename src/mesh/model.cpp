#include "model.hpp"

#include <iostream>

/// @brief Gets the amount of meshes in the model
/// @return Amount of meshes in the model
unsigned int Model::mesh_number() const
{
    return m_meshes.size();
}

/// @brief Gets the mesh m of the model
/// @param m Mesh index to get
/// @return Meshes m in the model
Mesh* Model::get_mesh(unsigned int m) const
{
    return m_meshes.at(m).get();
}

/// @brief Adds a mesh to the model
/// @param mesh Mesh to add
void Model::add_mesh(std::unique_ptr<Mesh> mesh)
{
    m_meshes.push_back(std::move(mesh));
}

/// @brief Draws the model
/// @param model Model matrix
void Model::draw(const glm::mat4& model) const
{
    for (auto &&m : m_meshes)
        m.get()->draw(model);
}