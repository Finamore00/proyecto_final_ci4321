#include "particle_system.hpp"
#include "emitter.hpp"

#include "../gl_utils/shader.hpp"

Emitter &ParticleSystemComponent::add_emitter()
{
    gl_utils::shader_program shader("", "");
    m_emitters.push_back(Emitter(*this, 30, shader));
    return m_emitters.back();
}

void ParticleSystemComponent::update(float dt)
{
}

ParticleSystemComponent::ParticleSystemComponent(SceneObject* sObj) : Component(sObj)
{
}