#include "ParticleSystem.h"

namespace Falcon
{

    ParticleSystem::ParticleSystem()
    {

    }

    ParticleSystem::~ParticleSystem()
    {
        for (auto& batch : m_batches)
        {
            delete batch;
        }
    }

    void ParticleSystem::addParticleBatch(ParticleBatch *particleBatch)
    {
        m_batches.push_back(particleBatch);
    }

    void ParticleSystem::update(float deltaTime)
    {
        for (auto& batch : m_batches)
        {
            batch->update(deltaTime);
        }
    }

    void ParticleSystem::draw(SpriteBatch *spriteBatch)
    {
        spriteBatch->begin();
        for (auto& batch : m_batches)
        {
            batch->draw(spriteBatch);
        }
        spriteBatch->end();
        spriteBatch->renderBatch();
    }
}