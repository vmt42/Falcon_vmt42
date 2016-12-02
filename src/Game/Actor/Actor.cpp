#include "Actor.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../Level/Level.h"

bool Actor::levelCollision(const std::vector<std::string> &level)
{
    std::vector<glm::vec2> collideTilePositions;

    //Check the corners of collision
    checkTilePosition(level, collideTilePositions, m_position.x, m_position.y);

    checkTilePosition(level, collideTilePositions, m_position.x + ACTOR_WIDTH, m_position.y);

    checkTilePosition(level, collideTilePositions, m_position.x, m_position.y + ACTOR_WIDTH);

    checkTilePosition(level, collideTilePositions, m_position.x + ACTOR_WIDTH, m_position.y + ACTOR_WIDTH);

    for (auto &pos : collideTilePositions)
        collideWithTile(pos);

    return (collideTilePositions.size() != 0);
}

void Actor::checkTilePosition(const std::vector<std::string> &level, std::vector<glm::vec2> &collideTilePositions,
                              float x, float y)
{
    glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_HEIGHT));

    //If outside of world bounds
    if (cornerPos.x < 0 || cornerPos.x >= level[4].size() || cornerPos.y < 0 || cornerPos.y >= level.size())
        return;

    if (level[cornerPos.y][cornerPos.x] != '.')
        collideTilePositions.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2(TILE_WIDTH / 2, TILE_HEIGHT / 2));
}

// AABB collision
void Actor::collideWithTile(glm::vec2 tilePos)
{
    const float TILE_RADIUS = TILE_WIDTH / 2;
    const float MIN_DISTANCE = ACTOR_RADIUS + TILE_RADIUS;

    glm::vec2 centerPlayerPos = m_position + glm::vec2(ACTOR_RADIUS);
    glm::vec2 distVec = centerPlayerPos - tilePos;

    float x_depth = MIN_DISTANCE - abs(distVec.x);
    float y_depth = MIN_DISTANCE - abs(distVec.y);


    if (x_depth > 0 || y_depth > 0)
    {
        if (std::max(x_depth, 0.0f) < std::max(y_depth, 0.0f))
        {
            if (distVec.x < 0)
            {
                m_position.x -= x_depth;
            }
            else
            {
                m_position.x += x_depth;
            }
        }
        else
        {
            if (distVec.y < 0)
            {
                m_position.y -= y_depth;
            }
            else
            {
                m_position.y += y_depth;
            }
        }
    }
}

// Circular collision
bool Actor::actorCollision(Actor* actor)
{
    const float MIN_DISTANCE = ACTOR_RADIUS * 2.0f;

    glm::vec2 centerPosA = m_position + glm::vec2(ACTOR_RADIUS);
    glm::vec2 centerPosB = actor->getPosition() + glm::vec2(ACTOR_RADIUS);
    glm::vec2 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);


    float collisionDepth = MIN_DISTANCE - distance;

    if (collisionDepth > 0)
    {
        glm::vec2 colDepthVec = glm::normalize(distVec) * collisionDepth;

        m_position += colDepthVec / 2.0f;
        actor->m_position -= colDepthVec / 2.0f;
        return true;
    }
    return false;
}

bool Actor::addDamage(float damage)
{
    m_health -= damage;
    m_color = m_color - Falcon::Color(0, 75, 75, 0);

    return m_health <= 0;
}
