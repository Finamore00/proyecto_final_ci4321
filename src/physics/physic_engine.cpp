#include "physic_engine.hpp"

#include <iostream>

#include "../scene_graph/transform.hpp"
#include "../scene_graph/sceneobject.hpp"

PhysicEngine *PhysicEngine::g_instance = nullptr;

PhysicEngine::PhysicEngine() {
    if (g_instance != nullptr) {
        throw;
    }

    g_instance = this;
}

PhysicEngine* PhysicEngine::get_instance()
{
    return g_instance;
}

/// @brief Syncs the transform properties of all the physics entities in the engine,
///        with the colliders properties.
void PhysicEngine::sync_transforms()
{
   for (auto &&ent : m_physicEnts)
   {
        if (ent.collider.type == SPHERE_COLLIDER)
        {
            ent.collider.shape.sphere.pos = ent.transform.get_world_position();
            continue;
        }

        ent.collider.shape.obb.center = ent.transform.get_world_position();
        ent.collider.shape.obb.axes[0] = ent.transform.get_right_vector();
        ent.collider.shape.obb.axes[1] = ent.transform.get_up_vector();
        ent.collider.shape.obb.axes[2] = ent.transform.get_front_vector();
        ent.collider.shape.obb.halfW = ent.transform.get_world_scale() / 2.0f;
   }
}

/// @brief Register a collider and a transform within the physic engine
/// @param collider Collider to register
/// @param transform Transform to register
void PhysicEngine::register_entity(Collider& collider, Transform& transform)
{
    m_physicEnts.push_back({collider, transform});
}

void PhysicEngine::register_entity(ColliderComponent* collider) 
{
    m_colliders.push_back(collider);
}

/// @brief Simulates a step in the physic engine.
///        At the moment this only syncs the transforms and
//         check for collisions.
void PhysicEngine::simulate()
{
    sync_transforms();
    // dumb n^2 solution
    for (auto e1 = m_physicEnts.cbegin(); e1 != m_physicEnts.cend(); e1++)
    {
        if (!e1->transform.get_scene_object().enabled)
            continue;

        for (auto e2 = e1 + 1; e2 != m_physicEnts.cend(); e2++)
        {
            if (!e2->transform.get_scene_object().enabled)
                continue;

            bool result = test_collision(e1->collider, e2->collider);
            if (!result)
                continue;

            // Should this happen here? I'm not sure. How do I handle them? How I handle destructions?
            std::cout << e1->transform.get_scene_object().get_ID() << " is in a collision with ";
            std::cout << e2->transform.get_scene_object().get_ID() << std::endl;
            
            e1->transform.get_scene_object().on_collision(e2->collider, e2->transform);
            e2->transform.get_scene_object().on_collision(e1->collider, e1->transform);
        }
    }
}