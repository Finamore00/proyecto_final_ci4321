#include "tank.hpp"
#include "../mesh/mesh.hpp"
#include "../mesh/geometry.hpp"
#include "../input/input.hpp"
#include "../physics/collision_primitives.hpp"
#include <iostream>

Tank::Tank(SceneObject &parent, gl_utils::shader_program &shader) {

    //The box will be the parent of all other shapes in the tank. So its parent must be set outside
    mesh = new Mesh(create_box(0.5f, 1.0f), shader);
    transform.set_parent(&parent.transform, true);
    enabled = true;


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

    //Bullet spawner
    SceneObject *bullet_spawner = new SceneObject;
    bullet_spawner->transform.set_parent(&turret_cylinder->transform, true);
    bullet_spawner->transform.set_local_euler_rotation(glm::vec3(90.0f, 0.0f, 0.0f));
    bullet_spawner->transform.set_local_position(glm::vec3(0.0f, 1.0f, 0.0f));
    spawner_transform = &bullet_spawner->transform;

    //Initiate bullets
    for (int i = 0; i < 3; i++) {
        bullets[i] = new Bullet(1.0f, -9.8f, true);
        bullets[i]->mesh = new Mesh(create_sphere(15, 10, 0.125f), shader);
        bullets[i]->transform.set_parent(&bullet_spawner->transform, false);
        bullets[i]->spawn(bullet_spawner->transform, 5.0f);
        bullets[i]->collider = new Collider;
        *(bullets[i]->collider) = create_sphere_collider(bullets[i]->transform, 0.125f);
        bullets[i]->enabled = false;
    }
}

void Tank::update(float time) {
    rotate_tank(time);
    move(time);
    rotate_turret(time);
    fire_bullet();

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

void Tank::fire_bullet() {

    InputManager *input;
    input = input->get_instance();

    if (input->key_is_pressed(GLFW_KEY_SPACE)) {
        float time = glfwGetTime();
        if (time - last_fired_time > 1.0f) {
            last_fired_time = time;

            // Search for the first disabled bullet
            int i = 0;
            while (i < 3 && bullets[i]->enabled == true) {
                i++;
            }

            if (i != 3) {
                bullets[i]->enabled = true;
                bullets[i]->spawn(*spawner_transform, 10.0f);
            }
        }
    }
}

void Tank::update_bullets(float time) {
    for (int i = 0; i < 3; i++) {
        if (bullets[i]->enabled) {
            bullets[i]->update(time);;
        }
    }
}