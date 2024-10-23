#pragma once
#include "../gl_utils/shader.h"
#include "geometry.hpp"
#include "material.hpp"

class Mesh
{
private:
    Geometry m_geometry;
    Material m_shaderMaterial;
    unsigned int VBO, VAO, EBO;

    void initializeGeometry(const Geometry& geometry);
    
public:
    const gl_utils::shader_program& shader;

    Mesh(const Geometry& geometry, const gl_utils::shader_program& shader);
    void draw();
};

#define VPOS_LAY_LOC 0
#define VPOS_LAY_NOR 1
#define VPOS_LAY_UVS 2