#pragma once
#include "sceneobject.hpp"

class Component {
private:
    void friend SceneObject::on_collision(Collider& collider, Transform& transform);
    
protected:
    SceneObject* m_sceneObj = nullptr;
    friend void SceneObject::add_component(Component& component);
    virtual void __on_collision(Collider& collider, Transform& transform) {};

public:
    Component();

    SceneObject& get_scene_object() {return *m_sceneObj;}
    virtual void update(float dt) {};
};