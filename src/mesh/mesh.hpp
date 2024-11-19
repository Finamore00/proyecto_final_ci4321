#pragma once
#include "../gl_utils/shader.hpp"
#include "geometry.hpp"
#include "material.hpp"

class Mesh
{
private:
    Geometry m_geometry;
    unsigned int m_VBO, m_VAO, m_EBO;

    
public:
    const gl_utils::shader_program* shader;
    Material shaderMaterial;

    Mesh(const Geometry& geometry, const gl_utils::shader_program& shader);
    void initialize_geometry(const Geometry& geometry);
    void set_shader(const gl_utils::shader_program& ns);
    void draw() const;
};

#define VPOS_LAY_LOC 0
#define VPOS_LAY_NOR 1
#define VPOS_LAY_UVS 2
#define VPOS_LAY_TAN 3
#define VPOS_LAY_BITAN 4