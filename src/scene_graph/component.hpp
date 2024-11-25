#pragma once

#include "sceneobject.hpp"

class ColliderComponent;

class Component {
protected:
    static unsigned int s_nextID;
    const unsigned int m_ID;
    SceneObject* m_sceneObj = nullptr;

    ~Component();
    
public:
    bool enabled = true;
    Component(SceneObject* sObj);

    unsigned int ID() const { return m_ID;}
    SceneObject& get_scene_object() {return *m_sceneObj;}
    virtual void update(float dt) {};
    virtual void __on_collision(ColliderComponent& collider, Transform& transform) = 0;
};