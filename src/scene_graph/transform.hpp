#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include <vector>
#include <memory>

class SceneObject;

class Transform
{
protected:
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_eulerRot = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};

    glm::mat4 m_modelMatrix = glm::mat4(1.0f);
    bool m_isDirty = true;


    std::vector<Transform*> children;
    Transform* parent = nullptr;
    SceneObject* sceneObject = nullptr;

    glm::mat4 getLocalModelMatrix();
    void computeModelMatrix();
    void computeModelMatrix(const glm::mat4& parentGlobalModelMat);

public:
    bool isDirty() const {return m_isDirty;}
    const glm::mat4& getModelMatrix() const {return m_modelMatrix;}

    const glm::vec3& getLocalPosition() {return m_position;}
    const glm::vec3& getLocalScale() {return m_position;}

    void setLocalPosition(const glm::vec3& position);
    void setLocalEulerRotation(const glm::vec3& position);
    void setLocalScale(const glm::vec3& scale);

    void setParent(Transform* const parent);
    void removeChild(Transform& child);

    void forceUpdateTransform();
    void updateTransform();
};

class SceneObject
{
public:
    Transform transform;
};