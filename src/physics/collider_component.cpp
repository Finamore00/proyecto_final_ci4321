#include "collider_component.hpp"
#include "physic_engine.hpp"

void ColliderComponent::update(float dt) 
{
    sync_transform(); // maybe check for dirty transform somehow...
}

BoxCollider::BoxCollider(const glm::vec3& extents)
{
    Transform& transform = m_sceneObj->transform;
    glm::vec3 scale = transform.get_world_scale();
    m_extents = extents;

    sync_transform();
    PhysicEngine::get_instance()->register_entity(this);
}

void BoxCollider::sync_transform() {
    Transform& transform = m_sceneObj->transform;
    glm::vec3 scale = transform.get_world_scale();

    m_obb.center = transform.get_world_position();

    m_obb.axes[0] = transform.get_right_vector();
    m_obb.axes[1] = transform.get_up_vector();
    m_obb.axes[2] = transform.get_front_vector();

    m_obb.halfW[0] = m_extents.x * scale.x / 2.0f;
    m_obb.halfW[1] = m_extents.y * scale.y / 2.0f;
    m_obb.halfW[2] = m_extents.z * scale.z / 2.0f;
}

void BoxCollider::set_dimensions(const glm::vec3& extents)
{
    m_extents = extents;
    sync_transform();
}

SphereCollider::SphereCollider(float radius)
{
    set_radius(radius);
    sync_transform();
    PhysicEngine::get_instance()->register_entity(this);
}

void SphereCollider::sync_transform()
{
    m_sphere.pos = m_sceneObj->transform.get_world_position();
}

void SphereCollider::set_radius(float radius)
{
    m_sphere.radius = radius;
}