#pragma once
#include <vector>
#include <map>
#include "collision_primitives.hpp"

struct Collider;
class Transform;
class ColliderComponent;

/// @brief Represents a physic entity in the physic simulation.
struct PhysicEntity {
    Collider& collider;
    Transform& transform;
};

class PhysicEngine {
private:
    static PhysicEngine *g_instance;
    std::vector<PhysicEntity> m_physicEnts;
    std::vector<ColliderComponent*> m_colliders;
    void sync_transforms();
public:
    PhysicEngine();

    static PhysicEngine* get_instance();
    
    void register_entity(Collider& collider, Transform& transform);
    void register_entity(ColliderComponent* collider);
    void simulate();
};