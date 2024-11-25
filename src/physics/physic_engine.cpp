#include "physic_engine.hpp"

#include <iostream>

#include "../scene_graph/transform.hpp"
#include "../scene_graph/sceneobject.hpp"

#include "collider_component.hpp"

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
    for (auto &&ent : m_colliders)
    {
        if (!ent->enabled || !ent->get_scene_object().active)
            continue;;

        ent->sync_transform();
    }
}

/// @brief Registers a collider into the physic engine
/// @param collider Collider to register
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
    for (auto e1 = m_colliders.cbegin(); e1 != m_colliders.cend(); e1++)
    {
        if (!(*e1)->get_scene_object().active || !(*e1)->enabled)
            continue;

        for (auto e2 = e1 + 1; e2 != m_colliders.cend(); e2++)
        {
            if (!(*e2)->get_scene_object().active || !(*e2)->enabled)
                continue;

            bool result = test_collision(**e1, **e2);
            if (!result)
                continue;

            // Should this happen here? I'm not sure. How do I handle them? How I handle destructions?
            std::cout << (*e1)->get_scene_object().get_ID() << " is in a collision with: ";
            std::cout << (*e2)->get_scene_object().get_ID() << std::endl;
            
            (*e1)->get_scene_object().on_collision(**e2, (*e2)->get_scene_object().transform);
            (*e2)->get_scene_object().on_collision(**e1, (*e1)->get_scene_object().transform);
        }
    }
}