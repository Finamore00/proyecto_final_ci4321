#pragma once
#include <vector>
#include <memory>
#include <typeinfo>
#include <iostream>

#include "transform.hpp"

class ColliderComponent;
class Component;
class Model;
class Mesh;

class SceneObject
{
protected:
    static unsigned int m_curId; 
    const unsigned int m_ID;
    std::vector<Component*> m_components;

public:
    bool active = true;

    Transform transform = Transform(*this);
    Mesh* mesh = nullptr;
    std::shared_ptr<Model> model;

    SceneObject();
    
    unsigned int get_ID() const {return m_ID;};
    virtual void on_collision(ColliderComponent& collider, Transform& transform);

    void add_component(Component& component);
    
    template <class T> Component* get_component();
};

/// @brief Retrieves a component attached to a scene object
/// @tparam T A subclass of the Component class
/// @return The first component of type T found. nullptr otherwise
template <class T>
Component* SceneObject::get_component()
{
    static_assert(std::is_base_of<Component, T>().value, "Trying to get_component a non component class");
    for (Component*& c : m_components)
    {
        if (typeid(*c) == typeid(T))
            return c;

        if (dynamic_cast<T*>(c) != nullptr)
            return c;
    }

    return nullptr;
}