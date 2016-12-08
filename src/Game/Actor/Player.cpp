#include "Player.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../Weapon/RangeWeapon.h"
#include "../Weapon/MeleeWeapon.h"
#include "../Level/Level.h"

Player::Player(glm::vec2 position, float speed, Falcon::InputManager *inputManager, Falcon::Camera2D *camera,
               std::vector<Bullet> *bullets, std::vector<Slash> *slash)
        : m_currentGun(-1), m_points(0)
{
    m_speed = speed;
    m_position = position;
    m_inputManager = inputManager;
    m_camera = camera;
    m_bullets = bullets;
    m_slashes = slash;
}

void Player::update(const std::vector<std::string> &level, std::vector<Human*>& humans, std::vector<Enemy*>& enemies, float deltaTime)
{
    //glm::vec2 centerPlayerPos = m_position + glm::vec2(8);
    if (m_inputManager->isKeyPressed(SDLK_w))
    {
        m_position.y += m_speed * deltaTime;
    }
    else if (m_inputManager->isKeyPressed(SDLK_s))
    {
        m_position.y -= m_speed * deltaTime;
    }

    if (m_inputManager->isKeyPressed(SDLK_a))
    {
        m_position.x -= m_speed * deltaTime;
    }
    else if (m_inputManager->isKeyPressed(SDLK_d))
    {
        m_position.x += m_speed * deltaTime;
    }

    if (m_inputManager->isKeyPressed(SDLK_1) && m_rangeWeapons.size() >= 0)
    {
        m_currentGun = 0;
    }
    else if (m_inputManager->isKeyPressed(SDLK_2) && m_rangeWeapons.size() >= 1)
    {
        m_currentGun = 1;
    }
    else if (m_inputManager->isKeyPressed(SDLK_3) && m_rangeWeapons.size() >= 2)
    {
        m_currentGun = 2;
    }
    else if (m_inputManager->isKeyPressed(SDLK_4))
    {
        m_currentGun = 3;
    }

    if (m_currentGun != -1)
    {
        glm::vec2 mouse = m_inputManager->getMouseCoords();
        mouse = m_camera->screenToWorld(mouse);

        glm::vec2 centerPosition = m_position + glm::vec2(ACTOR_RADIUS);
        glm::vec2 direction = glm::normalize(mouse - centerPosition);

        if (m_currentGun == 3)
        {
            m_meleeWeapon->update(m_inputManager->isKeyPressed(SDL_BUTTON_LEFT), centerPosition - glm::vec2(0, 32), direction,
                                  *m_slashes, deltaTime, 100 * 3.14159265359f / 180);
        }
        else
        {
            m_rangeWeapons[m_currentGun]->update(m_inputManager->isKeyPressed(SDL_BUTTON_LEFT), centerPosition, direction, *m_bullets, deltaTime);
        }

    }

    levelCollision(level);

}

void Player::draw(Falcon::SpriteBatch &spriteBatch)
{
    static GLuint shadowID = Falcon::ResourceManager::getTexture("media/Textures/shadow.png").id;
    static GLuint textureID = Falcon::ResourceManager::getTexture("media/Textures/player.png").id;
    glm::vec4 uvVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 posVecShadow(m_position.x, m_position.y - 10, TILE_WIDTH, TILE_WIDTH);
    glm::vec4 posVec(m_position.x, m_position.y, ACTOR_WIDTH, ACTOR_HEIGHT);
    spriteBatch.draw(posVecShadow, uvVec, shadowID, 0, Falcon::Color{164, 164, 164, 255});
    spriteBatch.draw(posVec, uvVec, textureID, 0, Falcon::Color{255, 255, 255, 255});
}

void Player::addRangeWeapon(RangeWeapon *rangeWeapon)
{
    m_rangeWeapons.push_back(rangeWeapon);

    if (m_currentGun == -1)
    {
        m_currentGun = 0;
    }
}

void Player::addMeleeWeapon(MeleeWeapon *meleeWeapon)
{
    m_meleeWeapon = meleeWeapon;
}

