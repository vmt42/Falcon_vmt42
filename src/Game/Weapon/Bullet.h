#ifndef FALCONLATINUM_BULLET_H
#define FALCONLATINUM_BULLET_H


#include <glm/vec2.hpp>
#include "../Actor/Enemy.h"

class Bullet
{
public:
    Bullet(float damage, glm::vec2 position, glm::vec2 direction, float speed)
            : m_damage(damage), m_position(position), m_direction(direction), m_speed(speed) {};

    void update(std::vector<Human*>& humans, std::vector<Enemy*>& enemies);
    void draw(Falcon::SpriteBatch& spriteBatch);

private:
    float m_damage;
    glm::vec2 m_position;
    glm::vec2 m_direction;
    float m_speed;
};


#endif
