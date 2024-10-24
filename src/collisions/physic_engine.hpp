#pragma once
#include <vector>
#include <map>
#include "collision_primitives.hpp"

struct Collider;
class Transform;

/// @brief Represents a physic entity in the physic simulation.
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