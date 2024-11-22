#pragma once

#include "sceneobject.hpp"

class ColliderComponent;

class Component {
protected:
    SceneObject* m_sceneObj = nullptr;

public:
    bool enabled = true;
    Component(SceneObject* sObj);

    SceneObject& get_scene_object() {return *m_sceneObj;}
    virtual void update(float dt) {};
    virtual void __on_collision(ColliderComponent& collider, Transform& transform) = 0;
};