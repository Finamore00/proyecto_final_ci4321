#include "mesh.hpp"
#include <iostream>

#include "../../thirdparty/glad/include/glad/glad.h"

Mesh::Mesh(const Geometry& geometry, const gl_utils::shader_program& shader): shader(&shader)
{
    initialize_geometry(geometry);
}

/// @brief Sets all of the mesh data (VBO, EBO, VAO) of a mesh using a geometry
/// @param geometry Geometry of the mesh
void Mesh::initialize_geometry(const Geometry& geometry)
{
    m_geometry = geometry;
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * m_geometry.vertices.size(), &(m_geometry.vertices[0]), GL_STATIC_DRAW);

    // Setting indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_geometry.indices.size(), &(m_geometry.indices[0]), GL_STATIC_DRAW); 

    // Assigning attributes
    glVertexAttribPointer(VPOS_LAY_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(VPOS_LAY_NOR, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(VPOS_LAY_UVS, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 2));
    glEnableVertexAttribArray(2);
}

void Mesh::set_shader(const gl_utils::shader_program& ns)
{
    this->shader = &ns;
}

/// @brief Draws the mesh
void Mesh::draw() const
{
    shader->use();
    if (shaderMaterial.albedo != nullptr)
        shaderMaterial.albedo->use_texture(GL_TEXTURE0);
    else
        glBindTexture(GL_TEXTURE_2D, 0);

    shader->set_vec3f("ambient", shaderMaterial.ambient);
    shader->set_vec3f("tint", shaderMaterial.tint);
    shader->set_vec2f("uvt.uvOffset", shaderMaterial.texUVoffset);
    shader->set_vec2f("uvt.uvScale", shaderMaterial.texUVscales);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_geometry.indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}