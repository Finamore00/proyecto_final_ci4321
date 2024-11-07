#pragma once
#include <vector>
#include <map>
#include "collision_primitives.hpp"

class Transform;
class ColliderComponent;

class PhysicEngine {
private:
    static PhysicEngine *g_instance;
    std::vector<ColliderComponent*> m_colliders;
public:
    PhysicEngine();

    static PhysicEngine* get_instance();
    void sync_transforms();
    
    void register_entity(ColliderComponent* collider);
    void simulate();
};