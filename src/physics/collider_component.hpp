#pragma once

#include "../scene_graph/component.hpp"
#include "collision_primitives.hpp"

class ColliderComponent: public Component {
protected:
    ColliderComponent(); // Not to be used!
    virtual void sync_transform() = 0;
public:
    void update(float dt) override;
};

class BoxCollider: public ColliderComponent {
private:
    OBB m_obb;
    glm::vec3 m_extents = glm::vec3(1.0f);

public:
    BoxCollider(const glm::vec3& extents);
    const OBB &get_shape() const { return m_obb; };
    void sync_transform() override;
    void set_dimensions(const glm::vec3& extents);
};

class SphereCollider: public ColliderComponent {
private:
    Sphere m_sphere;
public:
    SphereCollider(float radius);
    const Sphere& get_shape() const {return m_sphere;};
    void sync_transform() override;
    void set_radius(float radius);
};