#include "logic_engine.hpp"

#include "sceneobject.hpp"
#include "component.hpp"

void LogicEngine::register_component(SceneObject* obj, Component* comp)
{
    if (!m_comps.contains(obj))
    {
        m_comps[obj] = {comp};
        return;
    }

    m_comps[obj].push_back(comp);
}

void LogicEngine::update(float dt) const
{
    for (auto oiter = m_comps.begin(); oiter != m_comps.end(); oiter++)
    {
        if (!oiter->first->enabled)
            continue;

        for (auto citer = oiter->second.begin(); citer != oiter->second.end(); citer++)
            (*citer)->update(dt);
    }   
}