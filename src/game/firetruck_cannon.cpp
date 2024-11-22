#include "firetruck_cannon.hpp"

#include "../../input/input.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "../../thirdparty/glm/gtx/vector_angle.hpp"


FiretruckCannonComponent::FiretruckCannonComponent(SceneObject *sObj, Transform &cannon, Transform& bulletSpawn, const std::vector<SceneObject*>& bullets)
    : Component(sObj), m_cannon_pivot(cannon), m_bullet_spawn(bulletSpawn)
{
    for (auto &&b : bullets)
        m_bullets.push_back(b->get_component<BulletComponent>());
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

void FiretruckCannonComponent::shoot(bool grav)
{
    m_last_bullet = (m_last_bullet + 1) % m_bullets.size();
    std::weak_ptr<BulletComponent>bull = m_bullets.at(m_last_bullet);
    if (auto b = bull.lock())
    {
        b->get_scene_object().active = true;
        b->spawn(m_bullet_spawn, 20.0f, grav);
    }
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

    bool shootNormal = input->key_is_pressed(GLFW_KEY_SPACE);
    bool shootGrav = input->key_is_pressed(GLFW_KEY_LEFT_CONTROL);
    if (shootNormal || shootGrav)
    {
        float time = glfwGetTime();
        if (time - m_last_shot_time >= m_fire_rate)
        {
            m_last_shot_time = time;
            shoot(shootNormal ? !shootNormal : shootGrav);
        }
    }

    rotate_turret(turretRot, dt);
    rotate_cannon(cannonRot, dt);
}
