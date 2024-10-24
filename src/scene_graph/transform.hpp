#pragma once
#include <vector>

#include "../../thirdparty/glm/glm.hpp"
#include "../../thirdparty/glm/gtc/quaternion.hpp"

class SceneObject;
class Collider;
class Mesh;

class Transform
{
protected:
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::quat m_rotation = glm::quat(glm::vec3(0.0f));

    glm::mat4 m_modelMatrix = glm::mat4(1.0f);
    bool m_isDirty = true;

    std::vector<Transform*> m_children;
    Transform* m_parent = nullptr;
    SceneObject& m_sceneObject;

    glm::mat4 get_local_model_matrix();
    void compute_model_matrix();
    void compute_model_matrix(const glm::mat4& parentGlobalModelMat);

public:
    bool is_dirty() const {return m_isDirty;}
    const glm::mat4& get_model_matrix() const {return m_modelMatrix;}
    const std::vector<Transform*>& get_children() const {return m_children;}
    SceneObject& get_scene_object() const {return m_sceneObject;}

    const glm::vec3& get_local_scale() const {return m_position;}
    const glm::vec3& get_local_position() const {return m_position;}
    const glm::quat& get_local_rotation() const {return m_rotation;}
    glm::vec3 get_local_euler_rotation() const {return glm::eulerAngles(m_rotation);}

    glm::vec3 get_up_vector() const;
    glm::vec3 get_right_vector() const;
    glm::vec3 get_front_vector() const;

    void set_local_position(const glm::vec3& position);
    void set_local_rotation(const glm::quat& rot);
    void set_local_euler_rotation(const glm::vec3& euler);
    void set_local_scale(const glm::vec3& scale);

    glm::vec3 get_world_position() const;
    glm::quat get_world_rotation() const;
    glm::vec3 get_world_euler_rotation() const;
    glm::vec3 get_world_scale() const;
    
    void set_world_position(const glm::vec3& position);
    void set_world_rotation(const glm::quat& rot);
    void set_world_euler_rotation(const glm::vec3& euler);
    void set_world_scale(const glm::vec3& scale);

    void set_parent(Transform* const parent, bool keepWorld);
    void remove_child(Transform& child);

    void force_update_transform();
    void update_transform();

    Transform(SceneObject &obj);
};

class SceneObject
{
protected:
    static unsigned int m_curId; 
    const unsigned int m_ID;

public:
    bool enabled = true;

    Transform transform = Transform(*this);
    Collider* collider = nullptr;
    Mesh* mesh = nullptr;

    SceneObject();
    
    unsigned int get_ID() const {return m_ID;};
    virtual void on_collision(Collider& collider, Transform& transform) {};
};