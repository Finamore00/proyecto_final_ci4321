#include "speedo_meter_component.hpp"
#include "../../input/input.hpp"

SpeedoMeterComponent::SpeedoMeterComponent(SceneObject *sObj, float idleRot, float activeRot, Transform &target)
    : Component(sObj), m_idleRot(idleRot), m_activeRot(activeRot), m_rotTarget(target)
{
    m_rotTarget.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, idleRot));
}

void SpeedoMeterComponent::update(float dt)
{
    InputManager *input;
    input = input->get_instance();

    if (input->key_is_pressed(GLFW_KEY_W) || input->key_is_pressed(GLFW_KEY_S))
    {
        m_rotTarget.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, m_activeRot));
        return;
    }
    
    m_rotTarget.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, m_idleRot));
}
