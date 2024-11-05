#include "collision_primitives.hpp"

#include <iostream>
#include <typeinfo>

#include "collider_component.hpp"

bool test_collision(const ColliderComponent& a, const ColliderComponent& b)
{
    if (typeid(a) == typeid(SphereCollider))
    {
        const Sphere& aSphere = ((const SphereCollider&)(a)).get_shape();
        if (typeid(b) == typeid(SphereCollider))
            return test_sphere_sphere(aSphere, ((const SphereCollider&)(b)).get_shape());
        return test_sphere_OBB(aSphere, ((const BoxCollider&)(b)).get_shape());
    }

    if (typeid(b) == typeid(SphereCollider))
        return test_sphere_OBB(((const SphereCollider&)(b)).get_shape(), ((const BoxCollider&)(a)).get_shape());
    
    std::cout << "[WARNING] Collisions between OBBs not implemented yet" << std::endl;
    return false;
}

/// @brief Finds a point in an OBB surface, closest to a specified point
/// @param p Point 
/// @param b OBB
/// @return The point in b closest to p
glm::vec3 ClosestPtPointOBB(const glm::vec3& p, const OBB& b)
{
    glm::vec3 d = p - b.center;
    glm::vec3 res = b.center;

    for (int i = 0; i < 3; i++) {
        float dist = glm::dot(d, b.axes[i]);
        if (dist > b.halfW[i]) dist = b.halfW[i];
        if (dist < -b.halfW[i]) dist = -b.halfW[i];
        res += dist * b.axes[i];
    }
    return res;
}

/// @brief Tests for collision between two spheres
/// @param a Sphere a
/// @param b Sphere b
/// @return True if both spheres are colliding
bool test_sphere_sphere(const Sphere& a, const Sphere& b)
{
    glm::vec3 d = a.pos - b.pos;
    float rSum = a.radius + b.radius;
    return glm::dot(d, d) <= rSum * rSum;
}

/// @brief Tests for collision between a sphere and an OBB
/// @param s Sphere
/// @param o OBB
/// @return True if s and o are colliding
bool test_sphere_OBB(const Sphere& s, const OBB& o)
{
    glm::vec3 closest(ClosestPtPointOBB(s.pos, o));
    glm::vec3 v = closest - s.pos;
    return glm::dot(v, v) <= s.radius * s.radius;
}