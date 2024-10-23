#include "physic_engine.hpp"
#include <iostream>

void PhysicEngine::sync_transforms()
{
   for (auto &&ent : m_physicEnts)
   {
        if (ent.collider.type == SPHERE_COLLIDER)
        {
            ent.collider.shape.sphere.pos = ent.transform.getWorldPosition();
            continue;
        }

        ent.collider.shape.obb.center = ent.transform.getWorldPosition();
        ent.collider.shape.obb.axes[0] = ent.transform.getRightVector();
        ent.collider.shape.obb.axes[1] = ent.transform.getUpVector();
        ent.collider.shape.obb.axes[2] = ent.transform.getFrontVector();
        ent.collider.shape.obb.halfW = ent.transform.getWorldScale() / 2.0f;
   }
}
void PhysicEngine::register_entity(Collider& collider, Transform& transform)
{
    m_physicEnts.push_back({collider, transform});
}

void PhysicEngine::simulate()
{
    sync_transforms();
    // dumb n^2 solution
    for (auto e1 = m_physicEnts.cbegin(); e1 != m_physicEnts.cend(); e1++)
    {
        if (!e1->transform.getSceneObject().enabled)
            continue;

        for (auto e2 = e1 + 1; e2 != m_physicEnts.cend(); e2++)
        {
            if (!e2->transform.getSceneObject().enabled)
                continue;

            bool result = testCollision(e1->collider, e2->collider);
            if (!result)
                continue;

            // Should this happen here? I'm not sure. How do I handle them? How I handle destructions?
            std::cout << e1->transform.getSceneObject().getId() << " is in a collision with ";
            std::cout << e2->transform.getSceneObject().getId() << std::endl;
            
            e1->transform.getSceneObject().OnCollision(e2->collider, e2->transform);
            e2->transform.getSceneObject().OnCollision(e1->collider, e1->transform);
        }
    }
}