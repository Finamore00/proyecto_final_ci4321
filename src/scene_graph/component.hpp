#pragma once
#include "sceneobject.hpp"

class Component {
protected:
    SceneObject* m_sceneObj = nullptr;

public:
    Component();

    SceneObject& get_scene_object() {return *m_sceneObj;}
    virtual void update(float dt) {};
    friend void SceneObject::add_component(Component& component);
};