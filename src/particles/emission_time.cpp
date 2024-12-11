#include "emission_time.hpp"

#include <iostream>

bool EmissionTime::should_emit()
{
    return m_queued > 0;
}

unsigned int EmissionTime::queued_amount()
{
    return m_queued;
}

unsigned int EmissionTime::claim_queued(unsigned int claim)
{
    unsigned int claimed = claim <= m_queued ? claim : m_queued;
    m_queued -= claimed;
    return claimed;
}

BurstEmissionTime::BurstEmissionTime()
{}

BurstEmissionTime::BurstEmissionTime(unsigned int burstAmnt, float delay, float loop)
    : m_burstAmount(burstAmnt), m_burstDelay(delay), m_loop(loop) 
{}

void BurstEmissionTime::update(float dt)
{
    m_timer += dt;
    if (m_timer >= m_burstDelay)
    {
        if (m_done)
            return;

        m_queued += m_burstAmount;
        m_timer = 0.0f;
        if (!m_loop)
            m_done = true;
    }
}

void BurstEmissionTime::set_burst_amount(unsigned int amount)
{
    m_burstAmount = amount;
}
void BurstEmissionTime::set_burst_delay(float delay)
{
    if (delay < 0)
    {
        std::cout << "[BURST.EMISSOR] Delay set to negative value. Using absolute." << std::endl;
        delay = -delay;
    }

    m_burstDelay = delay;
}

void BurstEmissionTime::set_burst_loop(bool loop)
{
    m_loop = loop;
}

void BurstEmissionTime::reset()
{
    m_timer = 0.0f;
    m_done = false;
}

void RateEmissionTime::update(float dt)
{
    m_timer += dt;
    if (m_timer >= m_delay)
    {
        m_queued++;
        m_timer = 0.0f;
    }
}

void RateEmissionTime::set_rate(float rate)
{
    if (rate < 0.0f)
    {
        std::cout << "[RATE.EMISSOR] Rate set to negative value. Using absolute." << std::endl;
        rate = -rate;
    }

    m_rate = rate;
    m_delay = m_rate / m_amount;
}

void RateEmissionTime::set_amount(unsigned int amount)
{
    if (amount < 0.0f)
    {
        std::cout << "[RATE.EMISSOR] Amount set to zero value. Setting to one." << std::endl;
        amount = 1;
    }

    m_amount = amount;
    m_delay = m_rate / m_amount;
}

void RateEmissionTime::reset()
{
    m_timer = 0.0f;
}
