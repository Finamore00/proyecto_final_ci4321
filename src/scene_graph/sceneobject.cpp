#include "sceneobject.hpp"
#include <iostream>

#include "component.hpp"

unsigned int SceneObject::m_curId = 0u;
SceneObject::SceneObject() : m_ID(m_curId++) {}

void SceneObject::add_component(Component& component)
{
    if (component.m_sceneObj != nullptr)
    {
        std::cout << "[ERROR] Component already attached to a gameobject" << std::endl;
        return;
    }
    
    component.m_sceneObj = this;
    m_components.push_back(&component);
}