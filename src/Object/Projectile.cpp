#include "Projectile.h"
#include "../2D/GLTexture.h"
#include "../ResourceManager/ResourceManager.h"

namespace Falcon
{
    void Projectile::draw(Falcon::SpriteBatch &spriteBatch)
    {
        static Falcon::GLTexture texture = Falcon::ResourceManager::getTexture("images/Textures/Enemy_Mushroom1.png");
        glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
        Falcon::Color color;
        color.r = 255;
        color.g = 255;
        color.b = 255;
        color.a = 255;

        glm::vec4 sizeAndPosition = glm::vec4(m_position, 30, 30);

        spriteBatch.draw(sizeAndPosition, uv, texture.id, 0.0f, color);
    }

    bool Projectile::update()
    {
        m_position += m_direction * m_speed;
        m_lifeTime--;
        return m_lifeTime == 0;
    }

}
