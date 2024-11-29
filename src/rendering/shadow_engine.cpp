#include "shadow_engine.hpp"

void ShadowEngine::register_spotlight(const LightComponent& light, GLuint fb, GLuint tx)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    glBindTexture(GL_TEXTURE_2D, tx);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tx, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_fbs.emplace(light.ID(), light, fb, tx);
}

void ShadowEngine::register_directional(const LightComponent& light, GLuint fb, GLuint tx)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    glBindTexture(GL_TEXTURE_2D, tx);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tx, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_fbs.emplace(light.ID(), light, fb, tx);
}

void ShadowEngine::register_pointlight(const LightComponent& light, GLuint fb, GLuint tx)
{
}

void ShadowEngine::register_light(const LightComponent &light)
{
    unsigned int fb;
    glGenFramebuffers(1, &fb);

    unsigned int depthTx;
    glGenTextures(1, &depthTx);

    switch (light.get_type())
    {
    case LightSourceType::Spot: 
        register_spotlight(light, fb, depthTx);
        break;
    
    default:
        break;
    }
}

void ShadowEngine::prepare_light_shadow_pass(unsigned int id)
{
    if (m_fbs.find(id) == m_fbs.cend())
        return; // warning

    LightFBInfo& info = m_fbs[id];
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, info.fbId);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT_FACE);

    // render engine now should render the scene now!
}
