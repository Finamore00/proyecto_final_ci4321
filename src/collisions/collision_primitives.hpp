#pragma once
#include "../../thirdparty/glm/glm.hpp"

#define SPHERE_COLLIDER 0
#define OBB_COLLIDER 1

struct Sphere {
    float radius;
    glm::vec3 pos;
};

struct OBB {
    glm::vec3 center;
    glm::vec3 axes[3];
    glm::vec3 halfW;
};

union CollisionShapes
{
    Sphere sphere;
    OBB obb;
};


struct Collider {
    unsigned int type;
    CollisionShapes shape;
};

Collider createSphereCollider(float radius, const glm::vec3& pos);
bool testCollision(const Collider& a, const Collider& b);
bool testSphereOnSphere(const Sphere& a, const Sphere& b);
bool testSphereOnOBB(const Sphere& a, const OBB& b);