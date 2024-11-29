#include "light_component.hpp"

#include "rendering_engine.hpp"

LightComponent::LightComponent(SceneObject* sObj): Component(sObj) 
{
    RenderingEngine::get_instance()->register_light(*this);
}

void LightComponent::set_type(LightSourceType type)
{
    m_type = type;
    // Probably update light fbs and etc
}
