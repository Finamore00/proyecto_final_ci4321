#pragma once

class EmissionTime
{
protected:
    float m_timer = 0.0f;
    unsigned int m_queued = 0;
    
public:
    virtual void update(float dt) = 0;
    bool should_emit();
    unsigned int queued_amount();
    unsigned int claim_queued(unsigned int claim);
    virtual void reset() = 0;
};

class BurstEmissionTime: public EmissionTime
{
private:
    unsigned int m_burstAmount = 10;
    float m_burstDelay = 0.0f;
    bool m_loop = false;
    bool m_done = false;

public:
    BurstEmissionTime();
    BurstEmissionTime(unsigned int burstAmnt, float delay, float loop);

    void update(float dt) override;
    void set_burst_amount(unsigned int amount);
    void set_burst_delay(float delay);
    void set_burst_loop(bool loop);
    void reset();
};

class RateEmissionTime: public EmissionTime
{
private:
    float m_rate = 1.0f;
    unsigned int m_amount = 5u;
    float m_delay = 0.0f;

public:
    void update(float dt) override;
    void set_rate(float rate);
    void set_amount(unsigned int amount);
    void reset();
};