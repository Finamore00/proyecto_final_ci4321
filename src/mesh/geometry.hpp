#pragma once
#include <vector>
#include "../../thirdparty/glm/glm.hpp"

struct VertexData
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct Geometry
{
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indices;
};


Geometry createPlane(float x, float z);
Geometry createBox(float width, float height);
Geometry createSphere(unsigned int segments, unsigned int arcs, float radius);
Geometry createCylinder(unsigned int segments, float radius, float height);