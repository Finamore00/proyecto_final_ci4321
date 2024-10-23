#pragma once
#include <vector>
#include <map>
#include "../scene_graph/transform.hpp"
#include "collision_primitives.hpp"

struct PhysicEntity {
    Collider& collider;
    Transform& transform;
};

class PhysicEngine {
private:
    std::vector<PhysicEntity> m_physicEnts;
    void sync_transforms();
public:
    void register_entity(Collider& collider, Transform& transform);
    void simulate();
};