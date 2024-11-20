#include "component.hpp"

#include <iostream>

#include "logic_engine.hpp"

Component::Component(SceneObject* sObj): m_sceneObj(sObj) 
{
    LogicEngine::get_instance()->register_component(sObj, this);
};