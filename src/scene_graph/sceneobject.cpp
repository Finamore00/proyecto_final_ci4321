#include "sceneobject.hpp"
#include <iostream>

#include "component.hpp"

unsigned int SceneObject::m_curId = 0u;
SceneObject::SceneObject() : m_ID(m_curId++) {}

void SceneObject::on_collision(ColliderComponent& collider, Transform& transform)
{
    for (auto &&c : m_components) {
        if (!c->enabled)
            continue;

        c->__on_collision(collider, transform);
    }   
}