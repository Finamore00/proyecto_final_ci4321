#pragma once
#include "sceneobject.hpp"

class ColliderComponent;

class Component {
private:
    void friend SceneObject::on_collision(Collider& collider, Transform& transform);
    void friend SceneObject::on_collision(ColliderComponent& collider, Transform& transform);

protected:
    SceneObject* m_sceneObj = nullptr;
    friend void SceneObject::add_component(Component& component);
    virtual void __on_collision(Collider& collider, Transform& transform) {};
    virtual void __on_collision(ColliderComponent& collider, Transform& transform) = 0;

public:
    bool enabled = true;
    Component(SceneObject* sObj);

    SceneObject& get_scene_object() {return *m_sceneObj;}
    virtual void update(float dt) {};
};