#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include "../scene_graph/transform.hpp"

class ColliderComponent;

/// @brief Represents a sphere
struct Sphere {
    float radius;
    glm::vec3 pos;
};

/// @brief Represents an Object Oriented Box (OBB)
///        with a center, orientation axes and the scale
///        of those axes
struct OBB {
    glm::vec3 center;
    glm::vec3 axes[3];
    glm::vec3 halfW;
};

/// @brief Union holding the possible collision shapes
union CollisionShapes
{
    Sphere sphere;
    OBB obb;
};


bool test_collision(const ColliderComponent& a, const ColliderComponent& b);
bool test_sphere_sphere(const Sphere& a, const Sphere& b);
bool test_sphere_OBB(const Sphere& a, const OBB& b);