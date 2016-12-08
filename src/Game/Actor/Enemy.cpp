#include "Enemy.h"
#include "../../ResourceManager/ResourceManager.h"
#include "Human.h"
#include "../Level/Level.h"

Enemy::Enemy(glm::vec2 position, float speed)
{
    m_speed = speed;
    m_position = position;
    m_health = 100.0f;
    m_color = Falcon::Color(255, 255, 255, 255);
}

void Enemy::draw(Falcon::SpriteBatch &spriteBatch)
{
    static GLuint shadowID = Falcon::ResourceManager::getTexture("media/Textures/shadow.png").id;
    static GLuint textureID = Falcon::ResourceManager::getTexture("media/Textures/enemy.png").id;
    glm::vec4 uvVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 posVecShadow(m_position.x, m_position.y - 10, TILE_WIDTH, TILE_WIDTH);
    glm::vec4 posVec(m_position.x, m_position.y, ACTOR_WIDTH, ACTOR_HEIGHT);
    spriteBatch.draw(posVecShadow, uvVec, shadowID, 0, Falcon::Color{164, 164, 164, 255});
    spriteBatch.draw(posVec, uvVec, textureID, 0, m_color);
}

void Enemy::update(const std::vector<std::string> &level, std::vector<Human*>& humans, std::vector<Enemy*>& enemies, float deltaTime)
{

    Human* closestHuman = getNearestHuman(humans);

    if (closestHuman != nullptr)
    {
        glm::vec2 direction = glm::normalize(closestHuman->getPosition() - m_position);
        m_position += direction * m_speed * deltaTime;;
    }

    levelCollision(level);
}

Human *Enemy::getNearestHuman(std::vector<Human*> &humans)
{
    Human* closestHuman = nullptr;
    float min_distance = 1000000;

    for (auto &human : humans)
    {
        glm::vec2 distVec = human->getPosition() - m_position;
        float distance = glm::length(distVec);

        if (distance < min_distance)
        {
            min_distance = distance;
            closestHuman = human;
        }
    }

    return closestHuman;
}

