#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include "Human.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../Level/Level.h"

Human::Human(glm::vec2 position, float speed)
        : m_frames(0)
{
    static std::mt19937 RNG;
    static std::uniform_real_distribution<float> randomDirection(-1.0f, 1.0f);

    m_health = 20.0f;
    m_speed = speed;
    m_position = position;
    m_direction = glm::vec2(randomDirection(RNG), randomDirection(RNG));

    if (m_direction.length() == 0)
        m_direction = glm::vec2(1.0f, 0.0f);
    m_direction = glm::normalize(m_direction);

}

void Human::update(const std::vector<std::string> &level, std::vector<Human*>& humans, std::vector<Enemy*>& enemies, float deltaTime)
{
    static std::mt19937 RNG;
    static std::uniform_real_distribution<float> randomRotate(-40.0f * 3.14159265359f / 180, 40.0f * 3.14159265359f / 180);

    m_position += m_direction * m_speed * deltaTime;
    if (m_frames == 20)
    {
        m_direction = glm::rotate(m_direction, randomRotate(RNG));
        m_frames = 0;
    }
    else
    {
        m_frames++;
    }

    if (levelCollision(level))
    {
        m_direction = glm::rotate(m_direction, randomRotate(RNG));
    }

}

void Human::draw(Falcon::SpriteBatch &spriteBatch)
{
    static GLuint shadowID = Falcon::ResourceManager::getTexture("images/Textures/shadow.png").id;
    static GLuint textureID = Falcon::ResourceManager::getTexture("images/Textures/human.png").id;
    glm::vec4 uvVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 posVecShadow(m_position.x, m_position.y - 10, TILE_WIDTH, TILE_WIDTH);
    glm::vec4 posVec(m_position.x, m_position.y, ACTOR_WIDTH, ACTOR_HEIGHT);
    spriteBatch.draw(posVecShadow, uvVec, shadowID, 0, Falcon::Color{164, 164, 164, 255});
    spriteBatch.draw(posVec, uvVec, textureID, 0, Falcon::Color{255, 255, 255, 255});
}



