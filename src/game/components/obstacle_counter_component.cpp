#include "obstacle_counter_component.hpp"

ObstacleCounterComponent::ObstacleCounterComponent(SceneObject* sObj, const std::vector<SceneObject*>& obstacles, std::weak_ptr<FontComponent> text)
    : Component(sObj), m_obstacles(obstacles) , m_text(text) 
{
    m_count = m_obstacles.size();
    if (auto t = m_text.lock())
        t->set_text("Remaining obstacles: " + std::to_string(m_count));
}


void ObstacleCounterComponent::update(float dt)
{
    int remaining = m_obstacles.size();
    for (SceneObject* &o : m_obstacles)
        remaining += (o->active ? 0 : -1);

    if (remaining != m_count)
    {
        if (auto t = m_text.lock())
            t->set_text("Remaining obstacles: " + std::to_string(remaining));
            
        m_count = remaining;
    }
}