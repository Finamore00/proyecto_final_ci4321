#pragma once

#include <unordered_map>

#include "light_source_type.hpp"
#include "light_component.hpp"

#include "../../thirdparty/glad/include/glad/glad.h"
#include "../scene_graph/sceneobject.hpp"

struct LightFBInfo {
    LightComponent& light;
    GLuint fbId;
    GLuint txID;
};

class ShadowEngine 
{
private:
    std::unordered_map<unsigned int, LightFBInfo> m_fbs;

    void register_spotlight(const LightComponent& light, GLuint fb, GLuint tx);
    void register_directional(const LightComponent& light, GLuint fb, GLuint tx);
    void register_pointlight(const LightComponent& light, GLuint fb, GLuint tx);

public:
    void register_light(const LightComponent& light) {};
    void prepare_light_shadow_pass(unsigned int id);
};