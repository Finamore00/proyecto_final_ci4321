#pragma once
#include <vector>
#include <typeinfo>
#include <iostream>

#include "transform.hpp"

class Component;
class Collider;
class Mesh;

class SceneObject
{
protected:
    static unsigned int m_curId; 
    const unsigned int m_ID;
    std::vector<Component*> m_components;

public:
    bool enabled = true;

    Transform transform = Transform(*this);
    Collider* collider = nullptr;
    Mesh* mesh = nullptr;

    SceneObject();
    
    unsigned int get_ID() const {return m_ID;};
    virtual void on_collision(Collider& collider, Transform& transform) {};

    void add_component(Component& component);
    
    template <class T> Component* get_component();
};

template <class T>
Component* SceneObject::get_component()
{
    static_assert(std::is_base_of<Component, T>().value, "Trying to get_component a non component class");
    std::cout << "Buscando componente de tipo: " << typeid(T).name() <<std::endl;
    for (Component*& c : m_components)
    {
        std::cout << typeid(*c).name() << std::endl;
        if (typeid(*c) == typeid(T))
            return c;
    }

    return nullptr;
}