#include "firetruck_cannon.hpp"

#include "../../input/input.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "../../thirdparty/glm/gtx/vector_angle.hpp"

FiretruckCannonComponent::FiretruckCannonComponent(SceneObject *sObj, Transform& cannon) 
    : Component(sObj), m_cannon_pivot(cannon) 
{
}

void FiretruckCannonComponent::rotate_turret(float dir, float dt)
{
    if (dir == 0.0f)
        return;
    
    Transform& turret_transform = m_sceneObj->transform;
    glm::quat turret_y_rotation = turret_transform.get_world_rotation();

    glm::quat rotation;
    rotation = glm::angleAxis(glm::radians(70.0f) * dt * dir, glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::normalize(rotation);
    turret_transform.set_world_rotation(rotation * turret_y_rotation);
    turret_transform.update_transform();
}

void FiretruckCannonComponent::rotate_cannon(float dir, float dt)
{
    if (dir == 0.0f)
        return;
        
    float angBtw = glm::degrees(glm::angle(m_cannon_pivot.get_front_vector(), glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::quat pivot_x_rotation  = m_cannon_pivot.get_world_rotation();
    glm::vec3 right = m_cannon_pivot.get_right_vector();
    glm::quat rotation;
    if ((dir < 0.0f && (angBtw - glm::radians(40.0f) * dt * dir) > 1.0f) ||
        (dir > 0.0f && (angBtw + glm::radians(40.0f) * dt * dir) < 89.0f))
    {
        rotation = glm::angleAxis(glm::radians(40.0f) * dt * dir, right);
        rotation = glm::normalize(rotation);
        m_cannon_pivot.set_world_rotation(rotation * pivot_x_rotation);
        m_cannon_pivot.update_transform();
    }
}

void FiretruckCannonComponent::shoot()
{
}

void FiretruckCannonComponent::update(float dt)
{
    InputManager *input;
    input = input->get_instance();
    float turretRot = 0.0f;
    float cannonRot = 0.0f;

    if (input->key_is_pressed(GLFW_KEY_J))
        turretRot = 1.0f;
    else if (input->key_is_pressed(GLFW_KEY_L))
        turretRot = -1.0f;

    cannonRot = 0.0f;
    if (input->key_is_pressed(GLFW_KEY_I))
        cannonRot = -1.0f;
    else if (input->key_is_pressed(GLFW_KEY_K))
        cannonRot = 1.0f;

    rotate_turret(turretRot, dt);
    rotate_cannon(cannonRot, dt);
}
