#ifndef FALCONLATINUM_BLOOD_H
#define FALCONLATINUM_BLOOD_H


#include <glm/vec2.hpp>
#include "../../2D/SpriteBatch/SpriteBatch.h"
#include "../../ResourceManager/ResourceManager.h"

class Blood
{
public:
    Blood(glm::vec2 position) : m_position(position) { m_textureID = Falcon::ResourceManager::getTexture("media/Textures/blood_splat1.png").id;};
    void draw(Falcon::SpriteBatch& spriteBatch);
private:
    glm::vec2 m_position;
    GLuint m_textureID;
};


#endif
