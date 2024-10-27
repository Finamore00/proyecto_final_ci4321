#include "tank.hpp"
#include "../mesh/mesh.hpp"
#include "../mesh/geometry.hpp"
#include "../input/input.hpp"

Tank::Tank(SceneObject &parent, gl_utils::shader_program &shader) {

    //The box will be the parent of all other shapes in the tank. So its parent must be set outside
    this->mesh = new Mesh(create_box(0.5f, 1.0f), shader);
    this->transform.set_parent(&parent.transform, true);
    this->enabled = true;

    //Create sphere geometry for the turret
    SceneObject *turret_sphere = new SceneObject;
    turret_sphere->transform.set_parent(&this->transform, true);
    turret_sphere->transform.set_local_position(glm::vec3(0.25f, 0.0f, 0));
    turret_sphere->transform.set_local_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    turret_sphere->enabled = true;
    turret_sphere->mesh = new Mesh(create_sphere(30, 20, 0.25), shader);

    //Create pivot point for the cylinder
    SceneObject *cylinder_pivot = new SceneObject;
    cylinder_pivot->transform.set_parent(&turret_sphere->transform, true);
    turret_transform = &turret_sphere->transform;
    
    //Create the actual turret cylinder
    SceneObject *turret_cylinder = new SceneObject;
    turret_cylinder->mesh = new Mesh(create_cylinder(30, 0.0625f, 1.0f), shader);
    turret_cylinder->transform.set_parent(&cylinder_pivot->transform, true);
    turret_cylinder->transform.set_local_euler_rotation(glm::vec3(0.0f, 90.0f, 0.0f));
    turret_cylinder->transform.set_local_position(glm::vec3(0.0f, -0.5f, 0.0f));
    turret_cylinder->enabled = true;
}

void Tank::update(float time) {
    rotate_tank(time);
    move(time);
    rotate_turret(time);

    return;
}

void Tank::rotate_tank(float time) {
    InputManager *input;
    input = input->get_instance();

    glm::vec3 current_rotation = glm::eulerAngles(transform.get_world_rotation());
    const float rotation_delta = 3.141592 * time;

    if (input->key_is_pressed(GLFW_KEY_A)) {
        current_rotation.x += rotation_delta;
    }

    if (input->key_is_pressed(GLFW_KEY_D)) {
        current_rotation.x -= rotation_delta;
    }

    transform.set_world_rotation(glm::quat(current_rotation));
}

void Tank::move(float time) {
    InputManager *input;
    input = input->get_instance();

    glm::vec3 current_position = transform.get_world_position();
    glm::vec3 fwd = -transform.get_up_vector();
    const float move_delta = 1.0f * time;  // Decide on a maximum speed

    if (input->key_is_pressed(GLFW_KEY_W)) {
        current_position += move_delta * fwd;
    }

    if (input->key_is_pressed(GLFW_KEY_S)) {
        current_position -= move_delta * fwd;
    }

    transform.set_world_position(current_position);
}

//Turret rotates kinda weird rn
void Tank::rotate_turret(float time) {
    InputManager *input;
    input = input->get_instance();

    glm::vec3 curr_turret_rotation = glm::eulerAngles(turret_transform->get_local_rotation());
    const float rotation_delta = 3.141592f * time;

    //Check vertical rotation first
    if (input->key_is_pressed(GLFW_KEY_K)) {
        curr_turret_rotation.z -= rotation_delta;
    }

    if (input->key_is_pressed(GLFW_KEY_I)) {
        curr_turret_rotation.z += rotation_delta;
    }

    //Prevent turret from going into the tank
    curr_turret_rotation.z = curr_turret_rotation.z <= 0.0f ? 0.0f : curr_turret_rotation.z;
    curr_turret_rotation.z = curr_turret_rotation.z >= 3.141592 ? 3.141592 : curr_turret_rotation.z;

    //Check horizontal rotation
    if (input->key_is_pressed(GLFW_KEY_J)) {
        curr_turret_rotation.x -= rotation_delta;
    }

    if (input->key_is_pressed(GLFW_KEY_L)) {
        curr_turret_rotation.x += rotation_delta;
    }

    turret_transform->set_local_rotation(glm::quat(curr_turret_rotation));
    transform.update_transform();
}
