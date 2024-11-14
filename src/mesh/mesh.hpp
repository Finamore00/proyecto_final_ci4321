#pragma once

#include "../../thirdparty/glm/glm.hpp"

#include "../gl_utils/shader.h"
#include "geometry.hpp"
#include "material.hpp"

class Mesh
{
private:
    glm::mat4 m_transform = glm::mat4(0.0f);
    Geometry m_geometry;
    unsigned int m_VBO, m_VAO, m_EBO;
    
public:
    const gl_utils::shader_program* shader;
    Material shaderMaterial;

    Mesh(const glm::mat4& transform);
    Mesh(const Geometry& geometry, const glm::mat4& transform);
    Mesh(const Geometry& geometry, const gl_utils::shader_program& shader);
    void initialize_geometry(const Geometry& geometry);
    void set_shader(const gl_utils::shader_program& ns);
    glm::mat4 get_transform() const {return m_transform;}
    void draw(const glm::mat4& model) const;
};

#define VPOS_LAY_LOC 0
#define VPOS_LAY_NOR 1
#define VPOS_LAY_UVS 2