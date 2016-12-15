#include "Bullet.h"
#include "../Actor/Actor.h"
#include "../Actor/Human.h"
#include "../Actor/Enemy.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../Level/Level.h"

bool Bullet::update(const std::vector<std::string> &level, float deltaTime)
{
    m_position += m_direction * m_speed * deltaTime;

    return levelCollision(level);
}

void Bullet::draw(Falcon::SpriteBatch &spriteBatch)
{
    static GLuint textureID = Falcon::ResourceManager::getTexture("media/Textures/arrow.png").id;
    glm::vec4 uvVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 posVec(m_position.x, m_position.y, BULLET_RADIUS, BULLET_RADIUS);
    spriteBatch.draw(posVec, uvVec, textureID, 0, Falcon::Color{255, 255, 255, 255}, m_direction);
}

bool Bullet::actorCollision(Actor *actor)
{
    const float MIN_DISTANCE = ACTOR_RADIUS + BULLET_RADIUS;

    glm::vec2 centerPosA = m_position + glm::vec2(BULLET_RADIUS);
    glm::vec2 centerPosB = actor->getPosition() + glm::vec2(ACTOR_RADIUS);
    glm::vec2 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);
    float collisionDepth = MIN_DISTANCE - distance;

    return collisionDepth > 0;
}

bool Bullet::levelCollision(const std::vector<std::string> &level)
{
    glm::ivec2 cornerPos;
    cornerPos.x = floor(m_position.x / TILE_WIDTH);
    cornerPos.y = floor(m_position.y / TILE_WIDTH);

    //If outside of world bounds
    if (cornerPos.x < 0 || cornerPos.x >= level[4].size() || cornerPos.y < 0 || cornerPos.y >= level.size())
        return true;

    return (level[cornerPos.y][cornerPos.x] != '.');
}

