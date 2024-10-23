#include <iostream>

#include "transform.hpp"
#include "../../thirdparty/glm/gtc/matrix_transform.hpp"

unsigned int SceneObject::m_curId = 0u;

Transform::Transform(SceneObject &obj) : sceneObject(obj) {}
SceneObject::SceneObject() : m_ID(m_curId++) {}

glm::mat4 Transform::getLocalModelMatrix()
{
    const glm::mat4 rotMatrix = glm::mat4_cast(m_rotation);
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

glm::vec3 Transform::getUpVector() const
{
    return glm::normalize(glm::vec3(m_modelMatrix[1]));
}

glm::vec3 Transform::getRightVector() const
{
    return glm::normalize(glm::vec3(m_modelMatrix[0]));
}

glm::vec3 Transform::getFrontVector() const
{
    return glm::normalize(glm::vec3(m_modelMatrix[2]));
};

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

glm::vec3 Transform::getWorldPosition() const
{
    return glm::vec3(m_modelMatrix[3]);
}

glm::quat Transform::getWorldRotation() const
{
    return glm::quat(m_modelMatrix); // Not sure of this
}

glm::vec3 Transform::getWorldEulerRotation() const
{
    return glm::eulerAngles(glm::quat(m_modelMatrix));
}

glm::vec3 Transform::getWorldScale() const
{
    //if (m_isDirty)
    //    forceUpdateTransform(); // this should also update parents?
    const glm::mat4& s = parent->m_modelMatrix;
    return glm::vec3(
        s[0].length(),
        s[1].length(),
        s[2].length()
    );
}

void Transform::setWorldPosition(const glm::vec3& position)
{
    m_isDirty = true;
    if (parent == nullptr)
    {
        m_position = position;
        return;
    }

    const glm::mat4& p = parent->m_modelMatrix;
    glm::mat4 wPos = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 lPos = glm::inverse(p) * wPos;
    m_position = glm::vec3(lPos[3]);
}

void Transform::setWorldRotation(const glm::quat& rot)
{
    m_isDirty = true;
    if (parent == nullptr)
    {
        m_rotation = rot;
        return;
    }

    const glm::mat4& p = parent->m_modelMatrix; // find a way to cache this
    glm::mat4 wRot = glm::mat4_cast(rot);
    glm::mat4 lRot = glm::inverse(p) * wRot;
    m_rotation = glm::quat(lRot);
}

void Transform::setWorldEulerRotation(const glm::vec3& euler)
{
    m_isDirty = true;
    if (parent == nullptr)
    {
        m_rotation = glm::quat(euler);
        return;
    }

    const glm::mat4& p = parent->m_modelMatrix;
    glm::mat4 wRot = glm::mat4_cast(glm::quat(glm::radians(euler)));
    glm::mat4 lRot = glm::inverse(p) * wRot;
    m_rotation = glm::quat(lRot);
}

void Transform::setWorldScale(const glm::vec3& scale)
{
    m_isDirty = true;
    if (parent == nullptr)
    {
        m_scale = scale;
        return;
    }

    const glm::mat4& p = parent->m_modelMatrix;
    glm::mat4 wScale = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 lScale = glm::inverse(p) * wScale;
    m_scale = glm::vec3(
        glm::vec3(lScale[0]).length(), 
        glm::vec3(lScale[1]).length(), 
        glm::vec3(lScale[2]).length()
    ); // this isnt working :(
}

void Transform::removeChild(Transform& child)
{
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

void Transform::setParent(Transform* const newParent, bool keepWorld)
{
    if (parent != nullptr)
    {
        // This instance must remove itself from its parent instance!
        parent->removeChild(*this);
    }
    parent = newParent;
    if (newParent != nullptr)
        parent->children.emplace_back(this);

    m_isDirty = !keepWorld;
    if (!keepWorld)
        return;

    // check this
    setWorldPosition(getWorldPosition());
    setWorldRotation(getWorldRotation());
    setWorldScale(getWorldScale());
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
        computeModelMatrix(parent->m_modelMatrix);
    else
        computeModelMatrix();

    for (auto &&c : children)
        c->forceUpdateTransform();
}