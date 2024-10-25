#include <iostream>

#include "transform.hpp"
#include "../../thirdparty/glm/gtc/matrix_transform.hpp"

unsigned int SceneObject::m_curId = 0u;

Transform::Transform(SceneObject &obj) : m_sceneObject(obj) {}
SceneObject::SceneObject() : m_ID(m_curId++) {}

/// @brief Calculates the local model matrix
/// @return Local model matrix
glm::mat4 Transform::get_local_model_matrix()
{
    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
    const glm::mat4 rotMatrix = glm::mat4_cast(glm::normalize(m_rotation));
    const glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), m_position);

    return translateMatrix * rotMatrix * scaleMatrix;
}

/// @brief Computes the world model matrix with no parent
void Transform::compute_model_matrix()
{
    m_modelMatrix = get_local_model_matrix();
    m_isDirty = false;
}

/// @brief Computes the world model matrix with a parent
/// @param parentGlobalModelMat Parent model matrix
void Transform::compute_model_matrix(const glm::mat4& parentGlobalModelMat)
{
    m_modelMatrix = parentGlobalModelMat * get_local_model_matrix();
    m_isDirty = false;
}

/// @brief Gets world up vector
/// @return World up vector
glm::vec3 Transform::get_up_vector() const
{
    return glm::normalize(glm::vec3(m_modelMatrix[1]));
}

/// @brief Gets world right vector
/// @return World right vector
glm::vec3 Transform::get_right_vector() const
{
    return glm::normalize(glm::vec3(m_modelMatrix[0]));
}

/// @brief Gets world front vector
/// @return World front vector
glm::vec3 Transform::get_front_vector() const
{
    return glm::normalize(glm::vec3(m_modelMatrix[2]));
};

/// @brief Sets the local position of the transform
/// @param position Local position
void Transform::set_local_position(const glm::vec3& position)
{
    m_position = position;
    m_isDirty = true;
}

/// @brief Sets the local rotation of the transform using euler angles
/// @param euler Local Euler angles
void Transform::set_local_euler_rotation(const glm::vec3& euler)
{
    m_rotation = glm::quat(glm::radians(euler));
    m_isDirty = true;
}

/// @brief Sets the local rotation of the transform using a quaternion
/// @param rot Local quaternion
void Transform::set_local_rotation(const glm::quat& rot)
{
    m_rotation = rot;
    m_isDirty = true;
}

/// @brief Sets the local scale of the transform
/// @param scale Local scale
void Transform::set_local_scale(const glm::vec3& scale)
{
    m_scale = scale;
    m_isDirty = true;
}

/// @brief Gets the world position of the transform
/// @return World position
glm::vec3 Transform::get_world_position() const
{
    return glm::vec3(m_modelMatrix[3]);
}

/// @brief Gets the world rotation
/// @return World rotation
glm::quat Transform::get_world_rotation() const
{
    glm::mat3 rotationMatrix = glm::mat3(m_modelMatrix);
    
    rotationMatrix[0] = glm::normalize(rotationMatrix[0]);
    rotationMatrix[1] = glm::normalize(rotationMatrix[1]);
    rotationMatrix[2] = glm::normalize(rotationMatrix[2]);

    return glm::normalize(glm::quat_cast(rotationMatrix));
}

/// @brief Gets the world euler rotation
/// @return World euler rotation
glm::vec3 Transform::get_world_euler_rotation() const
{
    return glm::eulerAngles(glm::quat(m_modelMatrix));
}

/// @brief Gets the world scale
/// @return World scale
glm::vec3 Transform::get_world_scale() const
{
    //if (m_isDirty)
    //    forceUpdateTransform(); // this should also update parents?
    const glm::mat4& s = m_parent->m_modelMatrix;
    return glm::vec3(
        glm::length(glm::vec3(m_modelMatrix[0])),
        glm::length(glm::vec3(m_modelMatrix[1])),
        glm::length(glm::vec3(m_modelMatrix[2]))
    );
}

/// @brief Sets the world position of the transform
/// @param position Target world position
void Transform::set_world_position(const glm::vec3& position)
{
    m_isDirty = true;
    if (m_parent == nullptr)
    {
        m_position = position;
        return;
    }

    const glm::mat4& p = m_parent->m_modelMatrix;
    glm::mat4 wPos = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 lPos = glm::inverse(p) * wPos;
    m_position = glm::vec3(lPos[3]);
}

/// @brief Sets the world rotation of the transform using a quaternion
/// @param rot Target rotation quaternion
void Transform::set_world_rotation(const glm::quat& rot)
{
    m_isDirty = true;
    if (m_parent == nullptr)
    {
        m_rotation = rot;
        return;
    }

    glm::mat4 parentMatrix = m_parent->m_modelMatrix;
    glm::quat parentRotation = glm::quat_cast(parentMatrix);
    glm::quat localRotation = glm::inverse(parentRotation) * glm::normalize(rot);
    m_rotation = localRotation;
}

/// @brief Sets the world rotation of the transform using euler rotations
/// @param euler Target euler rotation
void Transform::set_world_euler_rotation(const glm::vec3& euler)
{
    m_isDirty = true;
    if (m_parent == nullptr)
    {
        m_rotation = glm::quat(euler);
        return;
    }

    const glm::mat4& p = m_parent->m_modelMatrix;
    glm::mat4 wRot = glm::mat4_cast(glm::quat(glm::radians(euler)));
    glm::mat4 lRot = glm::inverse(p) * wRot;
    m_rotation = glm::quat(lRot);
}

/// @brief Sets the world scale of the transform
/// @param scale Target world scale
void Transform::set_world_scale(const glm::vec3& scale)
{
    m_isDirty = true;
    if (m_parent == nullptr)
    {
        m_scale = scale;
        return;
    }

    const glm::mat4& p = m_parent->m_modelMatrix;
    glm::mat4 wScale = m_modelMatrix;
    wScale[0] = glm::vec4(glm::normalize(glm::vec3(wScale[0])), 0.0f) * scale.x;
    wScale[1] = glm::vec4(glm::normalize(glm::vec3(wScale[1])), 0.0f) * scale.y;
    wScale[2] = glm::vec4(glm::normalize(glm::vec3(wScale[2])), 0.0f) * scale.z;

    glm::mat4 lScale = glm::inverse(p) * wScale;
    m_scale = glm::vec3(
        glm::length(glm::vec3(lScale[0])), 
        glm::length(glm::vec3(lScale[1])), 
        glm::length(glm::vec3(lScale[2]))
    );
}

/// @brief Removes a child from the transform
/// @param child Child to remove
void Transform::remove_child(Transform& child)
{
    int i = 0;
    for (auto &&c : m_children)
    {
        if (c == &child) {
            m_children.erase(m_children.begin() + i);
            return;
        }
        i++;
    }
}

/// @brief Sets a transform as the parent
/// @param newParent New parent or null for none
/// @param keepWorld Keeps the world properties of the transform
void Transform::set_parent(Transform* const newParent, bool keepWorld)
{
    if (m_parent != nullptr)
    {
        // This instance must remove itself from its parent instance!
        m_parent->remove_child(*this);
    }
    m_parent = newParent;
    if (newParent != nullptr)
        m_parent->m_children.emplace_back(this);

    m_isDirty = !keepWorld;
    if (!keepWorld)
        return;

    // check this
    set_world_position(get_world_position());
    set_world_rotation(get_world_rotation());
    set_world_scale(get_world_scale());
}

/// @brief Updates the world matrix of the transform and
///        its children. Only updates the dirty ones
void Transform::update_transform()
{
    if (m_isDirty) {
        force_update_transform();
        return;
    }

    for (auto &&c : m_children)
        c->update_transform();
}

/// @brief Forces the updates the world matrix of the transform and
///        its children. Updates ALL the transforms
void Transform::force_update_transform()
{
    if (m_parent)
        compute_model_matrix(m_parent->m_modelMatrix);
    else
        compute_model_matrix();

    for (auto &&c : m_children)
        c->force_update_transform();
}