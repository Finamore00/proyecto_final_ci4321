#include "emission_shape.hpp"

#include <iostream>

#include "../../thirdparty/glm/gtc/random.hpp"
#include "../../thirdparty/glm/gtc/constants.hpp"

void EmissionShape::emit_range(unsigned int amount, std::vector<EmitedInfo> &target)
{
    for (unsigned int p = 0u; p < amount; ++p)
        target.emplace_back(emit());
}

EmitedInfo SphereEmissor::emit()
{
    glm::vec3 pos = glm::vec3(0.0f);
    if (random_pos)
        pos = glm::sphericalRand(m_radius);

    if (random_dir || !random_pos || (random_pos && pos.length() == 0))
    {
        glm::vec3 dir = glm::sphericalRand(m_radius);
        for(;dir.length() == 0.0f;dir = glm::sphericalRand(m_radius));
        return { pos, glm::normalize(dir) };
    }
    
    return { pos, glm::normalize(pos) };
}

void SphereEmissor::set_radius(float radius)
{
    if (radius < 0)
    {
        std::cout << "[SPHERE.EMISSOR] Radius set to negative value. Using absolute." << std::endl;
        radius = -radius;
    }

    m_radius = radius;
}

SphereEmissor::SphereEmissor() {}

SphereEmissor::SphereEmissor(float radius) : m_radius(radius) {}

void ConeEmissor::update_angle_rad()
{
    float hyp = glm::sqrt(m_baseRadius * m_baseRadius + m_height * m_height);
    m_angleRad = glm::asin(m_height / hyp);
}

ConeEmissor::ConeEmissor()
{
    set_radius(m_baseRadius);
}

ConeEmissor::ConeEmissor(float radius, float height) : m_baseRadius(radius), m_height(height)
{
    update_angle_rad();
}

EmitedInfo ConeEmissor::emit()
{
    float phi = glm::linearRand(0.0f, 2 * glm::pi<float>());
    float r = glm::linearRand(0.0f, m_baseRadius);
    glm::vec3 dir = glm::vec3(r * glm::cos(phi), r * glm::sin(phi), m_height);
    return { glm::vec3(0.0f), glm::normalize(dir) };
}

void ConeEmissor::set_radius(float radius)
{
    m_angleRad = radius;
    update_angle_rad();
}

void ConeEmissor::set_height(float height)
{
    m_height = height;
    update_angle_rad();
}
