#pragma once
#include <vector>
#include "../../thirdparty/glm/glm.hpp"
#define PI 3.14159274f

/// @brief Struct containing position, normal, and uv data of a vertex
struct VertexData
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

/// @brief Struct containing vertices data and indices of a geometry
struct Geometry
{
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indices;
};


Geometry create_plane(float x, float z);
Geometry create_box(float width, float height);
Geometry create_inverted_box(float width, float height);
Geometry create_sphere(unsigned int segments, unsigned int arcs, float radius);
Geometry create_cylinder(unsigned int segments, float radius, float height);