#include "component.hpp"

#include <iostream>

#include "logic_engine.hpp"

unsigned int Component::s_nextID = 0;

Component::Component(SceneObject* sObj): m_sceneObj(sObj), m_ID(s_nextID++)
{
    LogicEngine::get_instance()->register_component(this); // change this to weak_ptr??
}

Component::~Component()
{
    LogicEngine::get_instance()->unregister_component(m_ID);
}