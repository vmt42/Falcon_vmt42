#include "Slash.h"
#include "../../ResourceManager/ResourceManager.h"



bool Slash::update(float deltaTime)
{
    m_lifeTime -= 1.0f * deltaTime;
    return m_lifeTime == 0;
}

void Slash::draw(Falcon::SpriteBatch &spriteBatch)
{
    static GLuint textureID = Falcon::ResourceManager::getTexture("images/Textures/slash.png").id;
    glm::vec4 uvVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 posVec(m_position.x, m_position.y, SLASH_WIDTH, 96);
    spriteBatch.draw(posVec, uvVec, textureID, 0, Falcon::Color{255, 255, 255, 255});
}

bool Slash::actorCollision(Actor *actor)
{
    const float MIN_DISTANCE = ACTOR_RADIUS + SLASH_WIDTH / 2;

    glm::vec2 centerPosA = m_position + glm::vec2(SLASH_WIDTH / 2);
    glm::vec2 centerPosB = actor->getPosition() + glm::vec2(ACTOR_RADIUS);
    glm::vec2 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);
    float collisionDepth = MIN_DISTANCE - distance;

    return collisionDepth > 0;
}

