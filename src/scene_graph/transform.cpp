#include <iostream>

#include "transform.hpp"
#include "../../thirdparty/glm/gtc/matrix_transform.hpp"

Transform::Transform(SceneObject &obj) : sceneObject(obj) {}

glm::mat4 Transform::getLocalModelMatrix()
{
    const glm::mat4 rotMatrix = (glm::mat4)m_rotation;
    return glm::translate(glm::mat4(1.0f), m_position) * rotMatrix * glm::scale(glm::mat4(1.0f), m_scale);
}

void Transform::computeModelMatrix()
{
    m_modelMatrix = getLocalModelMatrix();
    m_isDirty = false;
}

void Transform::computeModelMatrix(const glm::mat4& parentGlobalModelMat)
{
    m_modelMatrix = parentGlobalModelMat * getLocalModelMatrix();
    m_isDirty = false;
}

void Transform::setLocalPosition(const glm::vec3& position)
{
    m_position = position;
    m_isDirty = true;
}

void Transform::setLocalEulerRotation(const glm::vec3& euler)
{
    m_rotation = glm::quat(glm::radians(euler));
    m_isDirty = true;
}

void Transform::setLocalRotation(const glm::quat& rot)
{
    m_rotation = rot;
    m_isDirty = true;
}

void Transform::setLocalScale(const glm::vec3& scale)
{
    m_scale = scale;
    m_isDirty = true;
}

void Transform::removeChild(Transform& child)
{
    // Check this out just in case
    int i = 0;
    for (auto &&c : children)
    {
        if (c == &child) {
            children.erase(children.begin() + i);
            return;
        }
        i++;
    }
}

void Transform::setParent(Transform* const newParent)
{
    if (parent != nullptr)
    {
        // This instance must remove itself from its parent instance!
        parent->removeChild(*this);
    }
    parent = newParent;
    if (newParent != nullptr)
        parent->children.emplace_back(this);

    m_isDirty = true;
}

void Transform::updateTransform()
{
    if (m_isDirty) {
        forceUpdateTransform();
        return;
    }

    for (auto &&c : children)
        c->updateTransform();
}

void Transform::forceUpdateTransform()
{
    if (parent)
        computeModelMatrix(parent->getModelMatrix());
    else
        computeModelMatrix();

    for (auto &&c : children)
        c->forceUpdateTransform();
}