#include "Player.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../Weapon/RangeWeapon.h"
void Player::init(glm::vec2 position, float speed, Falcon::InputManager* inputManager, Falcon::Camera2D* camera, std::vector<Bullet>* bullets)
{
    m_speed = speed;
    m_position = position + glm::vec2(8, 8);
    m_inputManager = inputManager;
    m_camera = camera;
    m_bullets = bullets;
}

void Player::update(const std::vector<std::string> &level, std::vector<Human*>& humans, std::vector<Enemy*>& enemies)
{
    //glm::vec2 centerPlayerPos = m_position + glm::vec2(8);
    if (m_inputManager->isKeyPressed(SDLK_w))
    {
        m_position.y += m_speed;
    }
    else if (m_inputManager->isKeyPressed(SDLK_s))
    {
        m_position.y -= m_speed;
    }

    if (m_inputManager->isKeyPressed(SDLK_a))
    {
        m_position.x -= m_speed;
    }
    else if (m_inputManager->isKeyPressed(SDLK_d))
    {
        m_position.x += m_speed;
    }

    if (m_inputManager->isKeyPressed(SDLK_1) && m_rangeWeapons.size() >= 0)
    {
        m_currentGun = 0;
    }
    else if (m_inputManager->isKeyPressed(SDLK_2) && m_rangeWeapons.size() >= 1)
    {
        m_currentGun = 1;
    }

    if (m_currentGun != -1)
    {

        glm::vec2 mouse = m_inputManager->getMouseCoords();
        mouse = m_camera->screenToWorld(mouse);

        glm::vec2 centerPosition = m_position + glm::vec2(ACTOR_RADIUS);
        glm::vec2 direction = glm::normalize(mouse - centerPosition);

        m_rangeWeapons[m_currentGun]->update(m_inputManager->isKeyPressed(SDL_BUTTON_LEFT), centerPosition, direction, *m_bullets);
    }

    levelCollision(level);

}

void Player::draw(Falcon::SpriteBatch &spriteBatch)
{
    static GLuint textureID = Falcon::ResourceManager::getTexture("images/Textures/player.png").id;
    glm::vec4 uvVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 posVec(m_position.x, m_position.y, ACTOR_WIDTH, ACTOR_HEIGHT);
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
