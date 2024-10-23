#pragma once
#include <vector>

#include "../../thirdparty/glm/glm.hpp"
#include "../../thirdparty/glm/gtc/quaternion.hpp"
#include "../collisions/collision_primitives.hpp"
#include "../mesh/mesh.hpp"

class SceneObject;

class Transform
{
protected:
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::quat m_rotation = glm::quat(glm::vec3(0.0f));

    glm::mat4 m_modelMatrix = glm::mat4(1.0f);
    bool m_isDirty = true;

    std::vector<Transform*> children;
    Transform* parent = nullptr;
    SceneObject& sceneObject;

    glm::mat4 getLocalModelMatrix();
    void computeModelMatrix();
    void computeModelMatrix(const glm::mat4& parentGlobalModelMat);

public:
    bool isDirty() const {return m_isDirty;}
    const glm::mat4& getModelMatrix() const {return m_modelMatrix;}
    const std::vector<Transform*>& getChildren() const {return children;}
    SceneObject& getSceneObject() const {return sceneObject;}

    const glm::vec3& getLocalScale() const {return m_position;}
    const glm::vec3& getLocalPosition() const {return m_position;}
    const glm::quat& getLocalRotation() const {return m_rotation;}
    glm::vec3 getLocalEulerRotation() const {return glm::eulerAngles(m_rotation);}

    glm::vec3 getUpVector() const;
    glm::vec3 getRightVector() const;
    glm::vec3 getFrontVector() const;

    void setLocalPosition(const glm::vec3& position);
    void setLocalRotation(const glm::quat& rot);
    void setLocalEulerRotation(const glm::vec3& euler);
    void setLocalScale(const glm::vec3& scale);

    glm::vec3 getWorldPosition() const;
    glm::quat getWorldRotation() const;
    glm::vec3 getWorldEulerRotation() const;
    glm::vec3 getWorldScale() const;
    
    void setWorldPosition(const glm::vec3& position);
    void setWorldRotation(const glm::quat& rot);
    void setWorldEulerRotation(const glm::vec3& euler);
    void setWorldScale(const glm::vec3& scale);

    void setParent(Transform* const parent, bool keepWorld);
    void removeChild(Transform& child);

    void forceUpdateTransform();
    void updateTransform();

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
    
    unsigned int getId() const {return m_ID;};
    virtual void OnCollision(Collider& collider, Transform& transform) {};
};