#include "particle_emitter.hpp"

#include "../../thirdparty/glm/gtc/matrix_transform.hpp"

ParticleEmitter::ParticleEmitter(SceneObject *sObj, gl_utils::shader_program &shader)
    : Component(sObj), m_shader(shader)
{
    m_particles = new Particle[m_maxParticles];

    prepare_buffers();

    m_pShape = std::make_shared<SphereEmissor>(0.5f);
    m_pTimer = std::make_shared<RateEmissionTime>();
}

ParticleEmitter::ParticleEmitter(SceneObject* sObj, unsigned int maxParticles, gl_utils::shader_program& shader)
    : Component(sObj), m_maxParticles(maxParticles), m_shader(shader)
{
    if (m_maxParticles == 0u)
        m_maxParticles = DEFAULT_MAX_PARTICLES;

    m_particles = new Particle[m_maxParticles];

    prepare_buffers();

    m_pShape = std::make_shared<SphereEmissor>(0.5f);
    m_pTimer = std::make_shared<RateEmissionTime>();
}

void ParticleEmitter::emit()
{
    unsigned int toClaim = m_pTimer->queued_amount();
    unsigned int available = m_maxParticles - m_firstAvailable;
    if (toClaim > available)
        toClaim = available;

    m_pTimer->claim_queued(toClaim);
    std::vector<EmitedInfo> startInfo;
    m_pShape->emit_range(toClaim, startInfo);

    unsigned int p = m_firstAvailable;
    for (auto &&pi : startInfo)
    {
        m_particles[p].position = pi.position;
        m_particles[p].velocity = pi.direction * m_pStartSpeed;
        
        m_particles[p].size = m_pSize;

        m_particles[p].startLifeTime = m_pLifeTime;
        m_particles[p].lifeTime = m_pLifeTime;

        m_particles[p].color = m_pStartColor;
        m_particles[p].startColor = m_pStartColor;
        m_particles[p].endColor = m_pEndColor;
        ++p;
    }
    
    m_firstAvailable = p;
    m_latestAlive = p - 1;
}

void ParticleEmitter::prepare_buffers()
{
    float quad[] = {
        // POS              // UVS
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
        0.5f, 0.5f, 0.0f,   1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    glGenBuffers(PARTICLES_VBOS_AMNT, m_VBOS);
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertices position and uvs
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_POS_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    // pos
    glEnableVertexAttribArray(PARTICLES_POS_ATT);
    glVertexAttribPointer(PARTICLES_POS_ATT, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    // uvs
    glEnableVertexAttribArray(PARTICLES_UVS_ATT);
    glVertexAttribPointer(PARTICLES_UVS_ATT, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

    // Particle colors
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_COL_VBO]);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(glm::vec4), nullptr, GL_STREAM_DRAW);
    glEnableVertexAttribArray(PARTICLES_COL_ATT);
    glVertexAttribPointer(PARTICLES_COL_ATT, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

    // Particle sizes
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_SIZE_VBO]);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(float), nullptr, GL_STREAM_DRAW);
    glEnableVertexAttribArray(PARTICLES_SIZE_ATT);
    glVertexAttribPointer(PARTICLES_SIZE_ATT, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

    // Model matrices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_MODEL_VBO]);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(glm::mat4), nullptr, GL_STREAM_DRAW);
    glEnableVertexAttribArray(PARTICLES_MODEL_ATT);
    glVertexAttribPointer(PARTICLES_MODEL_ATT, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(PARTICLES_MODEL_ATT + 1);
    glVertexAttribPointer(PARTICLES_MODEL_ATT + 1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)sizeof(glm::vec4));
    glEnableVertexAttribArray(PARTICLES_MODEL_ATT + 2);
    glVertexAttribPointer(PARTICLES_MODEL_ATT + 2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(PARTICLES_MODEL_ATT + 3);
    glVertexAttribPointer(PARTICLES_MODEL_ATT + 3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(PARTICLES_MODEL_ATT, 1);
    glVertexAttribDivisor(PARTICLES_MODEL_ATT + 1, 1);
    glVertexAttribDivisor(PARTICLES_MODEL_ATT + 2, 1);
    glVertexAttribDivisor(PARTICLES_MODEL_ATT + 3, 1);

    glBindVertexArray(0);
}

void ParticleEmitter::draw()
{
    if (m_firstAvailable == 0)
        return;

    glm::mat4 models[m_firstAvailable];
    glm::vec4 colors[m_firstAvailable];
    float sizes[m_firstAvailable];

    Transform& localTransform = m_sceneObj->transform;
    for (int p = 0; p < m_firstAvailable; ++p)
    {
        models[p] = localTransform.get_model_matrix() * glm::translate(glm::mat4(1.0f), m_particles[p].position);
        colors[p] = m_particles[p].color;
        sizes[p] = m_particles[p].size;
    }

    m_shader.use();
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_MODEL_VBO]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * m_firstAvailable, models);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_COL_VBO]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * m_firstAvailable, colors);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_SIZE_VBO]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_firstAvailable, sizes);

    if (m_texture != nullptr)
        m_texture.get()->use_texture(GL_TEXTURE0);
    else
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glDepthMask(GL_FALSE);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_firstAvailable);
    glDepthMask(GL_TRUE);
}

void ParticleEmitter::update(float dt)
{
    for (int p = 0u; p < m_firstAvailable; p++)
    {
        float nlf = m_particles[p].lifeTime - dt;
        float t = (m_particles[p].startLifeTime - nlf) / m_particles[p].startLifeTime;
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
        m_particles[p].color = glm::mix(m_particles[p].startColor, m_particles[p].endColor, t);
    }

    if (!m_playing)
        return;

    m_pTimer->update(dt);
    if (m_pTimer->should_emit())
        emit();
}

std::weak_ptr<EmissionShape> ParticleEmitter::get_shape()
{
    return m_pShape;
}

std::weak_ptr<EmissionTime> ParticleEmitter::get_timer()
{
    return m_pTimer;
}

void ParticleEmitter::start(bool reset)
{
    if (reset)
    {
        wipe();
        m_pTimer->reset();
    }

    m_playing = true;
}

void ParticleEmitter::stop()
{
    m_playing = false;
}

void ParticleEmitter::wipe()
{
    m_playing = false;
    //for (int p = 0u; p < m_firstAvailable; p++)
    //    m_particles->lifeTime = 0.0f;
    
    m_firstAvailable = 0;
    m_latestAlive = -1;
}

void ParticleEmitter::set_texture(std::shared_ptr<Texture> texture)
{
    m_texture = texture;
}

void ParticleEmitter::set_max_particles(unsigned int max)
{
    m_maxParticles = max;
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOS[PARTICLES_MODEL_ATT]);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(glm::mat4), nullptr, GL_STREAM_DRAW);
}

void ParticleEmitter::set_start_speed(float speed)
{
    m_pStartSpeed = speed;
}

void ParticleEmitter::set_start_color(const glm::vec4 col)
{
    m_pStartColor = col;
}

void ParticleEmitter::set_start_size(float size)
{
    if (size < 0.0f)
    {
        std::cout << "[PARTICLE SYSTEM] Trying to set the size of particles to negative value. ";
        std::cout << "Converting value to absolute." << std::endl;
        size = -size;
    }
    m_pSize = size;
}

void ParticleEmitter::set_end_color(const glm::vec4 col)
{
    m_pEndColor = col;
}

void ParticleEmitter::set_life_time(float life)
{
    if (life < 0.0f)
    {
        std::cout << "[PARTICLE SYSTEM] Trying to set the lifetime of particles to negative value. ";
        std::cout << "Converting value to absolute." << std::endl;
        life = -life; 
    }

    m_pLifeTime = life;
}
