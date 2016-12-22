
#include "Blood.h"
#include "../Level/Level.h"

void Blood::draw(Falcon::SpriteBatch &spriteBatch)
{
    glm::vec4 uvVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 posVec(m_position.x, m_position.y, TILE_WIDTH, TILE_HEIGHT);
    spriteBatch.draw(posVec, uvVec, m_textureID, 0, Falcon::Color{255, 255, 255, 255});
}
