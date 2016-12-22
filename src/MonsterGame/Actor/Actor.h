#ifndef FALCONLATINUM_ACTOR_H
#define FALCONLATINUM_ACTOR_H

#include <glm/vec2.hpp>
#include "../../2D/SpriteBatch/SpriteBatch.h"
#include <string>

class Enemy;
class Human;

const float ACTOR_WIDTH = 32.0f;
const float ACTOR_HEIGHT = 64.0f;
const float ACTOR_RADIUS = ACTOR_WIDTH / 2.0f;

class Actor
{
public:
    Actor() {};
    virtual ~Actor() {};

    virtual void update(const std::vector<std::string> &level, std::vector<Human*>& humans, std::vector<Enemy*>& enemies, float deltaTime) = 0;
    virtual void draw(Falcon::SpriteBatch& spriteBatch) = 0;

    bool levelCollision(const std::vector<std::string> &level);
    bool actorCollision(Actor* actor);
    bool addDamage(float damage);
    glm::vec2 getPosition() const { return m_position; };


protected:
    void checkTilePosition(const std::vector<std::string> &level, std::vector<glm::vec2>& collideTilePositions, float x, float y);
    void collideWithTile(glm::vec2 tilePos);

protected:
    float m_speed;
    glm::vec2 m_position;
    glm::vec2 m_direction;
    float m_health;
    Falcon::Color m_color;
};


#endif
