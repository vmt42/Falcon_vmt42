#include "ParticleBatch.h"


namespace Falcon
{
    ParticleBatch::ParticleBatch()
    {

    }

    ParticleBatch::~ParticleBatch()
    {
        delete[] m_particles;
    }

    void ParticleBatch::init(int maxParticles, float decayRate, GLTexture texture,
                             std::function<void(Particle &, float)> updateFunc /*= defaultParticleUpdate(*/)
    {
        m_maxParticles = maxParticles;
        m_particles = new Particle[maxParticles];
        m_decayRate = decayRate;
        m_texture = texture;
        m_updateFunc = updateFunc;
    }

    void ParticleBatch::update(float deltaTime)
    {
        for (int i = 0; i < m_maxParticles; i++)
        {
            if (m_particles[i].life > 0.0f)
            {
                m_updateFunc(m_particles[i], deltaTime);
                m_particles[i].life -= m_decayRate * deltaTime;
            }
        }
    }

    void ParticleBatch::draw(SpriteBatch* spriteBatch)
    {
        glm::vec4 uvRect(0.0, 0.0, 1.0f, 1.0f);
        for (int i = 0; i < m_maxParticles; i++)
        {
            auto& p = m_particles[i];
            if (p.life > 0.0f)
            {
                glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
                spriteBatch->draw(destRect, uvRect, m_texture.id, 0, p.color);
            }
        }
    }

    void ParticleBatch::addParticle(const glm::vec2 position, const glm::vec2 velocity, const Color color, float width)
    {
        int particleIndex = findFreeParticle();

        auto& p = m_particles[particleIndex];

        p.life = 1.0f;
        p.position = position;
        p.velocity = velocity;
        p.color = color;
        p.width = width;
    }

    int ParticleBatch::findFreeParticle()
    {
        for (int i = m_firstFreeParticle; i < m_maxParticles; i++)
        {
            if (m_particles[i].life <= 0.0f)
            {
                m_firstFreeParticle = i;
                return i;
            }
        }

        for (int i = 0; i < m_maxParticles; i++)
        {
            if (m_particles[i].life <= 0.0f)
            {
                m_firstFreeParticle = i;
                return i;
            }
        }

        return 0;
    }




}