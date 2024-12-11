#pragma once

#include <memory>

#include "emission_shape.hpp"
#include "emission_time.hpp"

#include "../../thirdparty/glm/glm.hpp"

#include "../scene_graph/transform.hpp"
#include "../scene_graph/component.hpp"
#include "../textures/texture.hpp"
#include "../gl_utils/shader.hpp"

#define PARTICLES_ATT_AMNT 5 // POS and UVS as 1
#define PARTICLES_VBOS_AMNT 4 // POS and UVS as 1

#define PARTICLES_POS_ATT 0
#define PARTICLES_UVS_ATT 1
#define PARTICLES_SIZE_ATT 2
#define PARTICLES_COL_ATT 3
#define PARTICLES_MODEL_ATT 4

#define PARTICLES_POS_VBO 0
#define PARTICLES_UVS_VBO 0
#define PARTICLES_SIZE_VBO 1
#define PARTICLES_COL_VBO 2
#define PARTICLES_MODEL_VBO 3

#define DEFAULT_MAX_PARTICLES 100

class ParticleSystemComponent;

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;

    glm::vec4 color;
    glm::vec4 startColor;
    glm::vec4 endColor;
    
    float size;

    float lifeTime;
    float startLifeTime;
};

class ParticleEmitter: public Component 
{
private:
    bool m_playing = false;

    Particle* m_particles;

    std::shared_ptr<EmissionShape> m_pShape;
    std::shared_ptr<EmissionTime> m_pTimer;

    unsigned int m_maxParticles = DEFAULT_MAX_PARTICLES;
    int m_latestAlive = 0u;
    int m_firstAvailable = 0u;
    gl_utils::shader_program& m_shader;

    GLuint m_VAO, m_EBO;
    GLuint m_VBOS[PARTICLES_ATT_AMNT];

    // Particle properties
    float m_pLifeTime = 5.0f;
    std::shared_ptr<Texture> m_texture = nullptr;
    float m_pStartSpeed = 1.0f;
    float m_pSize = 1.0f;
    glm::vec4 m_pStartColor = glm::vec4(1.0f);
    glm::vec4 m_pEndColor = glm::vec4(1.0f);

    virtual void emit();
    void prepare_buffers();

protected:
    void __on_collision(ColliderComponent& collider, Transform& transform) override {};

public:
    ParticleEmitter(SceneObject* sObj, gl_utils::shader_program& shader);
    ParticleEmitter(SceneObject* sObj, unsigned int maxParticles, gl_utils::shader_program& shader);

    std::weak_ptr<EmissionShape> get_shape();
    std::weak_ptr<EmissionTime> get_timer();

    void start(bool reset);
    void stop();
    void wipe();

    void set_texture(std::shared_ptr<Texture> texture);
    void set_max_particles(unsigned int max);
    void set_start_speed(float speed);
    void set_start_color(const glm::vec4 col);
    void set_start_size(float size);
    void set_end_color(const glm::vec4 col);
    void set_life_time(float life);

    template <class T, class... Args>
    void set_shape(Args &&...args);
    template <class T, class... Args>
    void set_timer(Args &&...args);

    void draw();
    void update(float dt);
};

template <class T, class... Args>
inline void ParticleEmitter::set_shape(Args &&...args)
{
    static_assert(std::is_base_of<EmissionShape, T>().value, "Trying to set_shape a non EmissionShape class");
    m_pShape = std::make_shared<T>(std::forward<Args>(args)...);
}

template <class T, class... Args>
inline void ParticleEmitter::set_timer(Args &&...args)
{
    static_assert(std::is_base_of<EmissionTime, T>().value, "Trying to set_timer a non EmissionTime class");
    m_pTimer = std::make_shared<T>(std::forward<Args>(args)...);
}
