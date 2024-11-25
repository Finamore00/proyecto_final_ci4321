#include "logic_engine.hpp"

#include "sceneobject.hpp"
#include "component.hpp"

LogicEngine *LogicEngine::g_instance = nullptr;

LogicEngine::LogicEngine()
{
    if (g_instance != nullptr) {
        throw;
    }

    g_instance = this;
}

LogicEngine* LogicEngine::get_instance()
{
    return g_instance;
}

void LogicEngine::register_component(Component* comp)
{
    if (m_comps.find(comp->ID()) != m_comps.end())
    {
        std::cout << "[WARNING] Trying to register an already registered component with ID: " << comp->ID() << std::endl;
        return;
    }

    m_comps[comp->ID()] = comp;
}

void LogicEngine::unregister_component(unsigned int id)
{
    if (m_comps.find(id) == m_comps.end())
    {
        std::cout << "[WARNING] Trying to unregister a non registered component with ID: " << id << std::endl;
        return;
    }

    m_comps.erase(id);
}

void LogicEngine::update(float dt) const
{
    std::cout << "[WARNING] NOT USING WEAK_PTR FOR LOGIC UPDATE. CHANGE ASAP" << std::endl;
    for (auto oiter = m_comps.begin(); oiter != m_comps.end(); oiter++)
    {
        Component* c = oiter->second;
        if (!c->get_scene_object().active)
            continue;

        if (!c->enabled)
            continue;

        c->update(dt);
    }   
}