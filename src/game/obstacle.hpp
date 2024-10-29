#pragma once
#include "../scene_graph/transform.hpp"
#include "../scene_graph/sceneobject.hpp"
#include "../mesh/mesh.hpp"

class Obstacle : public SceneObject {
public:
    void on_collision(Collider &collider, Transform &transform) override;
};

Obstacle create_sphere_obstacle(Transform &parent, unsigned int segments, unsigned int arcs, float radius, gl_utils::shader_program &shader, Texture *texture);
Obstacle create_box_obstacle(Transform &parent, float x, float y, float z, gl_utils::shader_program &shader, Texture &texture);
