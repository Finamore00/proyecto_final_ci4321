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
    std::vector<std::shared_ptr<Component>> m_components_s;

public:
    bool active = true;

    Transform transform = Transform(*this);
    Mesh* mesh = nullptr;
    std::shared_ptr<Model> model;

    SceneObject();
    
    unsigned int get_ID() const {return m_ID;};
    virtual void on_collision(ColliderComponent& collider, Transform& transform);

    template <class T,  class... Args> void add_component(Args&&... args);
    template <class T> std::weak_ptr<T> get_component();
};

template <class T, class... Args>
void SceneObject::add_component(Args &&...args)
{
    static_assert(std::is_base_of<Component, T>().value, "Trying to add_component a non component class");
    m_components_s.push_back(std::make_shared<T>(std::forward<Args>(args)...));
}

template <class T>
std::weak_ptr<T> SceneObject::get_component()
{
    static_assert(std::is_base_of<Component, T>().value, "Trying to get_component a non component class");
    for (auto &&c : m_components_s)
    {
        if (typeid(*c) == typeid(T))
            return std::dynamic_pointer_cast<T>(c);

        if (dynamic_cast<T*>(c.get()) != nullptr)
            return std::dynamic_pointer_cast<T>(c);
    }

    return std::weak_ptr<T>();
}