#include "mesh.hpp"
#include <glad/glad.h>

Mesh::Mesh(const Geometry& geometry, const gl_utils::shader_program& shader): shader(shader)
{
    initializeGeometry(geometry);
}

void Mesh::initializeGeometry(const Geometry& geometry)
{
    m_geometry = geometry;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * m_geometry.vertices.size(), &(m_geometry.vertices[0]), GL_STATIC_DRAW);

    // Setting indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_geometry.indices.size(), &(m_geometry.indices[0]), GL_STATIC_DRAW); 

    // Assigning attributes
    glVertexAttribPointer(VPOS_LAY_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(VPOS_LAY_NOR, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(VPOS_LAY_UVS, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 2));
    glEnableVertexAttribArray(2);
}

void Mesh::draw()
{
    glBindVertexArray(VAO);

    if (m_shaderMaterial.texture != nullptr)
        m_shaderMaterial.texture->useTexture(GL_TEXTURE0);

    shader.use();
    glDrawElements(GL_TRIANGLES, m_geometry.indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}