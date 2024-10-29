#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include "../scene_graph/transform.hpp"

#define SPHERE_COLLIDER 0
#define OBB_COLLIDER 1

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

/// @brief Collider. Specifies the type of the colliders
///        and stores the collision shape itself.
struct Collider {
    unsigned int type;
    CollisionShapes shape;
};

Collider create_sphere_collider(float radius, const glm::vec3& pos);
Collider create_OBB_collider(const glm::vec3& pos, const glm::vec3 up, const glm::vec3 right, glm::vec3 front, const glm::vec3& halfW);
Collider create_sphere_collider(const Transform& t, float radius);
Collider create_OBB_collider(const Transform& t);
bool test_collision(const Collider& a, const Collider& b);
bool test_collision(const ColliderComponent& a, const ColliderComponent& b);
bool test_sphere_sphere(const Sphere& a, const Sphere& b);
bool test_sphere_OBB(const Sphere& a, const OBB& b);