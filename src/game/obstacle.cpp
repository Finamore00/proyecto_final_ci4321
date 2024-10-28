#include "obstacle.hpp"
#include "../physics/collision_primitives.hpp"

void Obstacle::on_collision(Collider &collider, Transform &transform) {
    enabled = false;
}

Obstacle create_sphere_obstacle(
    Transform &parent, 
    unsigned int segments, 
    unsigned int stacks, float radius, 
    gl_utils::shader_program &shader,
    Texture &texture
) {
    Obstacle obstacle;

    obstacle.transform.set_parent(&parent, true);
    obstacle.mesh = new Mesh(create_sphere(segments, stacks, radius), shader);
    obstacle.mesh->shaderMaterial.texture = &texture;
    obstacle.collider = new Collider;
    *(obstacle.collider) = create_sphere_collider(obstacle.transform, radius);
    obstacle.enabled = false;

    return obstacle;
}

Obstacle create_box_obstacle(
    Transform &parent,
    float x,
    float y,
    float z,
    gl_utils::shader_program &shader,
    Texture &texture
) {
    Obstacle obstacle;

    obstacle.transform.set_parent(&parent, true);
    obstacle.mesh = new Mesh(create_box(x, y, z), shader);
    obstacle.mesh->shaderMaterial.texture = &texture;
    obstacle.collider = new Collider;
    *(obstacle.collider) = create_OBB_collider(obstacle.transform);
    obstacle.enabled = false;

    return obstacle;
}