#include "collision_primitives.hpp"
#include <iostream>

Collider createSphereCollider(float radius, const glm::vec3& pos)
{
    Collider res;
    res.type = SPHERE_COLLIDER;
    res.shape.sphere = Sphere{radius, pos};
    return res;
}

Collider createOBBCollider(const glm::vec3& pos, const glm::vec3 up, const glm::vec3 right, glm::vec3 front, const glm::vec3& halfW)
{
    Collider res;
    res.type = OBB_COLLIDER;
    res.shape.obb = OBB{pos, {right, up, front}, halfW};
    return res;
}

Collider createSphereCollider(const Transform& t, float radius)
{
    return createSphereCollider(radius, t.getWorldPosition());
}

Collider createOBBCollider(const Transform& t)
{
    return createOBBCollider(
        t.getWorldPosition(),
        t.getRightVector(), 
        t.getUpVector(), 
        t.getFrontVector(),
        t.getWorldScale() / 2.0f
    );
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
    //std::cout << "Sphere p:" << glm::to_string(s.pos) << std::endl;
    //std::cout << "Sphere r:" << s.radius << std::endl;
    //std::cout << "OBB p:" << glm::to_string(o.center) << std::endl;
    //std::cout << "OBB x:" << glm::to_string(o.axes[0]) << std::endl;
    //std::cout << "OBB y:" << glm::to_string(o.axes[1]) << std::endl;
    //std::cout << "OBB z:" << glm::to_string(o.axes[2]) << std::endl;
    //std::cout << "OBB hw:" << glm::to_string(o.halfW) << std::endl;
    //std::cout << "==============================" << std::endl;
    glm::vec3 v = closest - s.pos;
    return glm::dot(v, v) <= s.radius * s.radius;
}