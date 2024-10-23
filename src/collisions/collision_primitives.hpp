#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include "../scene_graph/transform.hpp"

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
Collider createOBBCollider(const glm::vec3& pos, const glm::vec3 up, const glm::vec3 right, glm::vec3 front, const glm::vec3& halfW);
Collider createSphereCollider(const Transform& t, float radius);
Collider createOBBCollider(const Transform& t);
bool testCollision(const Collider& a, const Collider& b);
bool testSphereOnSphere(const Sphere& a, const Sphere& b);
bool testSphereOnOBB(const Sphere& a, const OBB& b);