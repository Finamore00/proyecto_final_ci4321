#include "emitter.hpp"
#include "particle_system.hpp"

#include "../../thirdparty/glm/gtc/matrix_transform.hpp"

void Emitter::emit()
{
}

void Emitter::draw()
{
    if (m_firstAvailable == 0)
        return;

    glm::mat4 models[m_firstAvailable];
    for (int p = 0; p < m_firstAvailable; ++p)
        models[p] = m_localTransform.get_model_matrix() * glm::translate(glm::mat4(1.0f), m_particles[p].position);

    m_shader.use();
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_MODEL_ATT]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * m_firstAvailable, models);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_firstAvailable);
}

void Emitter::update(float dt)
{
    for (int p = 0u; p < m_firstAvailable; p++)
    {
        float nlf = m_particles[p].lifeTime - dt;
        if (nlf < 0.0f)
        {
            Particle tmp = m_particles[p];
            tmp.lifeTime = 0.0f;

            // Swap
            m_particles[p] = m_particles[m_latestAlive];
            m_particles[m_latestAlive] = tmp;

            m_firstAvailable--;
            m_latestAlive--; 
            p--;
            continue;
        }
        
        m_particles[p].lifeTime = nlf;
        m_particles[p].position += m_particles[p].velocity * dt;
    }
}

Emitter::Emitter(ParticleSystemComponent &owner, unsigned int maxParticles,  gl_utils::shader_program& shader)
    : m_owner(owner), m_maxParticles(maxParticles), m_shader(shader), m_localTransform(owner.get_scene_object().transform)
{
    // DEBUGGING
    m_latestAlive = 10;
    float quad[] = {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    float indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    m_particles = new Particle[maxParticles];

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(PARTICLES_ATT_AMNT, m_VBOS);
    
    glBindVertexArray(m_VAO);
    // Posicion de los vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_POS_ATT]);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(PARTICLES_POS_ATT);
    glVertexAttribPointer(PARTICLES_POS_ATT, 3, GL_FLOAT, GL_FALSE, 0,(void*)0);

    // Matrices de modelo
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_MODEL_ATT]);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(glm::vec4), nullptr, GL_STREAM_DRAW);
    glEnableVertexAttribArray(PARTICLES_MODEL_ATT);
    glVertexAttribPointer(PARTICLES_MODEL_ATT, 4, GL_FLOAT, GL_FALSE, 0,(void*)0);
    glEnableVertexAttribArray(PARTICLES_MODEL_ATT + 1);
    glVertexAttribPointer(PARTICLES_MODEL_ATT + 1, 4, GL_FLOAT, GL_FALSE, 0,(void*)sizeof(glm::vec4));
    glEnableVertexAttribArray(PARTICLES_MODEL_ATT + 2);
    glVertexAttribPointer(PARTICLES_MODEL_ATT + 2, 4, GL_FLOAT, GL_FALSE, 0,(void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(PARTICLES_MODEL_ATT + 3);
    glVertexAttribPointer(PARTICLES_MODEL_ATT + 3, 4, GL_FLOAT, GL_FALSE, 0,(void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(PARTICLES_MODEL_ATT, 1);
    glVertexAttribDivisor(PARTICLES_MODEL_ATT + 1, 1);
    glVertexAttribDivisor(PARTICLES_MODEL_ATT + 2, 1);
    glVertexAttribDivisor(PARTICLES_MODEL_ATT + 3, 1);
}

void Emitter::set_start_velocity(const glm::vec3 vel)
{
    m_pStartVel = vel;
}

void Emitter::set_start_color(const glm::vec4 col)
{
    m_pColor = col;
}

void Emitter::set_life_time(float life)
{
    if (life < 0.0f)
    {
        std::cout << "[PARTICLE SYSTEM] Trying to set the lifetime of particles to negative value. ";
        std::cout << "Converting value to absolute value." << std::endl;
        life = -life; 
    }

    m_pLifeTime = life;
}
