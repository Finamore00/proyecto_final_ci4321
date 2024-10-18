#pragma once
#include "geometry.hpp"

class Mesh
{
private:
    Geometry m_geometry;
    unsigned int VBO, VAO, EBO;

    void initializeMesh(const Geometry& geometry);
    
public:
    Mesh(const Geometry& geometry);
    Mesh();

    void draw();
    void setGeometry(const Geometry& geometry);
};