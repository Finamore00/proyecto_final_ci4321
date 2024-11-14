#include "model.hpp"

#include <iostream>

unsigned int Model::mesh_number() const
{
    return m_meshes.size();
}

Mesh* Model::get_mesh(unsigned int m) const
{
    return m_meshes.at(m).get();
}

void Model::add_mesh(std::unique_ptr<Mesh> mesh)
{
    m_meshes.push_back(std::move(mesh));
}

void Model::draw(const glm::mat4& model) const
{
    std::cout << "[MODEL] Drawing mesh" << std::endl;
    for (auto &&m : m_meshes)
        m.get()->draw(model);
}