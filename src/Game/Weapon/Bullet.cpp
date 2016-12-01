#include "Bullet.h"
#include "../../ResourceManager/ResourceManager.h"



void Bullet::update(std::vector<Human *> &humans, std::vector<Enemy *> &enemies)
{
    m_position += m_direction * m_speed;
}

void Bullet::draw(Falcon::SpriteBatch &spriteBatch)
{

    static GLuint textureID = Falcon::ResourceManager::getTexture("images/Textures/arrow.png").id;
    glm::vec4 uvVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 posVec(m_position.x, m_position.y, 6, 20);
    spriteBatch.draw(posVec, uvVec, textureID, 0, Falcon::Color{255, 255, 255, 255});
}
