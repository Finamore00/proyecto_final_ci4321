#pragma once

#include <vector>

#include "../../thirdparty/glm/glm.hpp"

struct EmitedInfo
{
    glm::vec3 position;
    glm::vec3 direction;
};

class EmissionShape
{
public:
    bool random_pos = false;
    bool random_dir = false;

    virtual EmitedInfo emit() = 0;
    void emit_range(unsigned int amount, std::vector<EmitedInfo>& target);
};

class SphereEmissor: public EmissionShape
{
private:
    float m_radius = 0.5f;
public:
    SphereEmissor();
    SphereEmissor(float radius);
    
    EmitedInfo emit() override;
    void set_radius(float radius);
};

class ConeEmissor: public EmissionShape
{
private:
    float m_baseRadius = 0.5f;
    float m_height = 1.0f;
    float m_angleRad;

    void update_angle_rad();
    
public:
    ConeEmissor();
    ConeEmissor(float radius, float height);
    
    EmitedInfo emit() override;
    void set_radius(float radius);
    void set_height(float height);
};