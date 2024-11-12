#include "destroyable_component.hpp"

void DestroyableComponent::__on_collision(ColliderComponent& collider, Transform& transform)
{
    m_sceneObj->active = false;
}