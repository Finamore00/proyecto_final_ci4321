#include "firetruck_component.hpp"

#include "../../input/input.hpp"

FiretruckComponent::FiretruckComponent(SceneObject* sObj) : Component(sObj)
{
}

void FiretruckComponent::rotate_tank(float dir, float dt)
{
    Transform& transform = m_sceneObj->transform;
    glm::quat current_rotation = transform.get_world_rotation();
    glm::vec3 up = glm::normalize(transform.get_up_vector());

    glm::quat rotation = glm::angleAxis(glm::radians(dir * 60.0f * dt), up);
    rotation = glm::normalize(rotation);

    transform.set_world_rotation(current_rotation * rotation);
}

void FiretruckComponent::move(const glm::vec3 &dir, float dt)
{
    Transform& transform = m_sceneObj->transform;
    glm::vec3 current_position = transform.get_world_position() + dir * 5.0f * dt;
    transform.set_world_position(current_position);
}

void FiretruckComponent::update(float dt)
{
    InputManager *input;
    input = input->get_instance();

    float rotationInput = 0.0f;
    float movementInput = 0.0f;

    if (input->key_is_pressed(GLFW_KEY_A))
        rotationInput = 1;
    else if (input->key_is_pressed(GLFW_KEY_D))
        rotationInput = -1;

    if (input->key_is_pressed(GLFW_KEY_W))
        movementInput = 1;
    else if (input->key_is_pressed(GLFW_KEY_S))
        movementInput = -1;

    move(m_sceneObj->transform.get_front_vector() * movementInput, dt);
    if (movementInput != 0.0f)
        rotate_tank(rotationInput * movementInput, dt);
}