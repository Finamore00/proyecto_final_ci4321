#include "tank.hpp"
#include "../mesh/mesh.hpp"
#include "../mesh/geometry.hpp"

Tank::Tank(SceneObject &parent, gl_utils::shader_program &shader) {

    //The box will be the parent of all other shapes in the tank. So its parent must be set outside
    this->mesh = new Mesh(create_box(0.5f, 1.0f), shader);
    this->transform.set_parent(&parent.transform, true);
    this->enabled = true;

    //Create sphere geometry for the turret
    SceneObject *turret_sphere = new SceneObject;
    turret_sphere->transform.set_parent(&this->transform, true);
    turret_sphere->transform.set_local_position(glm::vec3(0.25f, 0.0f, 0));
    turret_sphere->enabled = true;
    turret_sphere->mesh = new Mesh(create_sphere(30, 20, 0.25), shader);

    //Create pivot point for the cylinder
    SceneObject *cylinder_pivot = new SceneObject;
    cylinder_pivot->transform.set_parent(&turret_sphere->transform, true);
    
    //Create the actual turret cylinder
    SceneObject *turret_cylinder = new SceneObject;
    turret_cylinder->mesh = new Mesh(create_cylinder(30, 0.0625f, 0.5f), shader);
    turret_cylinder->transform.set_parent(&cylinder_pivot->transform, true);
    turret_cylinder->transform.set_local_euler_rotation(glm::vec3(0.0f, 90.0f, 0.0f));
    turret_cylinder->transform.set_local_position(glm::vec3(0.03125f, -0.25f, 0.0f));
    turret_cylinder->enabled = true;
}