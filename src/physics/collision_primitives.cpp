#include "collision_primitives.hpp"

#include <iostream>

/// @brief Creates a sphere collider
/// @param radius Sphere radius
/// @param pos Sphere center
/// @return Sphere collider with the radius and center specified
Collider create_sphere_collider(float radius, const glm::vec3& pos)
{
    Collider res;
    res.type = SPHERE_COLLIDER;
    res.shape.sphere = Sphere{radius, pos};
    return res;
}

/// @brief Create an Oriented Bounding Box (OBB) collider with
///        a specific center, up, right, front axes, and half
//         sizes for every axis.
/// @param pos Center
/// @param up Up axis
/// @param right Right axis
/// @param front Front axis
/// @param halfW Half size for every axis
/// @return Resulting OBB with the parameters specified
Collider create_OBB_collider(const glm::vec3& pos, const glm::vec3 up, const glm::vec3 right, glm::vec3 front, const glm::vec3& halfW)
{
    Collider res;
    res.type = OBB_COLLIDER;
    res.shape.obb = OBB{pos, {right, up, front}, halfW};
    return res;
}

/// @brief Creates a sphere collider centered in the position of
///        a specific transform, and an arbitrary radius
/// @param t Transform used to create sphere collider 
/// @param radius Radius of the sphere collider
/// @return Sphere collider centered in the transform and with the specified radius
Collider create_sphere_collider(const Transform& t, float radius)
{
    return create_sphere_collider(radius, t.get_world_position());
}

/// @brief Creates an OBB using a transform. It uses the world position as the center,
///        the world axes as the OBB axes, and its world scale as the axes scales.
/// @param t Transform used to create the OBB collider
/// @return OBB collider created using t
Collider create_OBB_collider(const Transform& t)
{
    return create_OBB_collider(
        t.get_world_position(),
        t.get_right_vector(), 
        t.get_up_vector(), 
        t.get_front_vector(),
        t.get_world_scale() / 2.0f
    );
}

/// @brief Test the collision between two colliders
/// @param a Collider a
/// @param b Collider b
/// @return True if there's a collision
bool test_collision(const Collider& a, const Collider& b)
{
    if (a.type == SPHERE_COLLIDER)
    {
        if (b.type == SPHERE_COLLIDER)
            return test_sphere_sphere(a.shape.sphere, b.shape.sphere);
        return test_sphere_OBB(a.shape.sphere, b.shape.obb);
    }

    if (b.type == SPHERE_COLLIDER)
        return test_sphere_OBB(b.shape.sphere, a.shape.obb);
    
    throw std::invalid_argument("Both colliders are OBBs");
};

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