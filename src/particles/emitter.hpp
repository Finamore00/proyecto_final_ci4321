#pragma once

#include "../../thirdparty/glm/glm.hpp"

#include "../scene_graph/transform.hpp"
#include "../gl_utils/shader.hpp"

#define PARTICLES_ATT_AMNT 4
#define PARTICLES_POS_ATT 0
#define PARTICLES_UVS_ATT 1
#define PARTICLES_COL_ATT 2
#define PARTICLES_MODEL_ATT 3

class ParticleSystemComponent;

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 color;
    float size;
    float lifeTime;
};

class Emitter 
{
private:
    ParticleSystemComponent& m_owner;
    Transform m_localTransform;
    Particle* m_particles;

    unsigned int m_maxParticles;
    int m_latestAlive = 0u;
    int m_firstAvailable = 0u;
    gl_utils::shader_program& m_shader;

    GLuint m_VAO, m_EBO;
    GLuint m_VBOS[PARTICLES_ATT_AMNT];

    float m_pLifeTime = 5.0f;
    float m_pSize = 1.0f;
    glm::vec4 m_pColor = glm::vec4(1.0f);
    glm::vec3 m_pStartVel = glm::vec3(1.0f);

    virtual void emit();
public:
    Emitter(ParticleSystemComponent& owner, unsigned int maxParticles, gl_utils::shader_program& shader);

    void set_start_velocity(const glm::vec3 vel);
    void set_start_color(const glm::vec4 col);
    void set_life_time(float life);


    void draw();
    void update(float dt);
};