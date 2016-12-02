#ifndef FALCONLATINUM_SLASH_H
#define FALCONLATINUM_SLASH_H


#include "../../2D/SpriteBatch/SpriteBatch.h"
#include "../Actor/Actor.h"
#include <string>


const float SLASH_WIDTH = 64;

class Slash
{
public:
    Slash(float damage, glm::vec2 position, glm::vec2 direction)
            : m_damage(damage), m_position(position), m_direction(direction), m_lifeTime(30.0f) {};
    void draw(Falcon::SpriteBatch& spriteBatch);
    bool update(float deltaTime);
    bool actorCollision(Actor *actor);
    float getDamage() const { return m_damage; };

private:
    float m_damage;
    glm::vec2 m_position;
    glm::vec2 m_direction;
    int m_lifeTime;
};


#endif
