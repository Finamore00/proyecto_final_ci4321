#include "collision_primitives.hpp"
#include <stdexcept>

Collider createSphereCollider(float radius, const glm::vec3& pos)
{
    return {
        SPHERE_COLLIDER,
        Sphere{radius, pos}
    };
}

bool testCollision(const Collider& a, const Collider& b)
{
    if (a.type == SPHERE_COLLIDER)
    {
        if (b.type == SPHERE_COLLIDER)
            return testSphereOnSphere(a.shape.sphere, b.shape.sphere);
        return testSphereOnOBB(a.shape.sphere, b.shape.obb);
    }

    if (b.type == SPHERE_COLLIDER)
        return testSphereOnOBB(b.shape.sphere, a.shape.obb);
    
    throw std::invalid_argument("Both colliders are OBBs");
};

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

bool testSphereOnSphere(const Sphere& a, const Sphere& b)
{
    glm::vec3 d = a.pos - b.pos;
    float rSum = a.radius + b.radius;
    return glm::dot(d, d) <= rSum * rSum;
}

bool testSphereOnOBB(const Sphere& s, const OBB& o)
{
    glm::vec3 closest(ClosestPtPointOBB(s.pos, o));
    glm::vec3 v = closest - s.pos;
    return glm::dot(v, v) <= s.radius * s.radius;
}