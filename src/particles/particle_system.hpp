#pragma once

#include <vector>

#include "../scene_graph/component.hpp"

class Emitter;

class ParticleSystemComponent: public Component 
{
private:
    std::vector<Emitter> m_emitters;

public:
    Emitter& add_emitter();
    void update(float dt);
    ParticleSystemComponent(SceneObject* sObj);
};