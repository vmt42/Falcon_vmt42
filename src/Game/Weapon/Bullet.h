#ifndef FALCONLATINUM_BULLET_H
#define FALCONLATINUM_BULLET_H


#include <glm/vec2.hpp>
#include "../Actor/Enemy.h"

static float BULLET_RADIUS = 10.0f;

class Actor;

class Bullet
{
public:
    Bullet(float damage, glm::vec2 position, glm::vec2 direction, float speed)
            : m_damage(damage), m_position(position), m_direction(direction), m_speed(speed) {};

    bool update(const std::vector<std::string> &level, float deltaTime); // If true - delete bullet
    void draw(Falcon::SpriteBatch& spriteBatch);
    bool actorCollision(Actor* actor);
    float getDamage() const { return m_damage; };

private:
    bool levelCollision(const std::vector<std::string> &level);

private:
    float m_damage;
    glm::vec2 m_position;
    glm::vec2 m_direction;
    float m_speed;
};


#endif
