#ifndef FALCONLATINUM_ENEMY_H
#define FALCONLATINUM_ENEMY_H


#include "Actor.h"

class Enemy : public Actor
{
public:
    Enemy(glm::vec2 position, float speed);
    ~Enemy() {};

    void draw(Falcon::SpriteBatch& spriteBatch) override;
    void update(const std::vector<std::string> &level, std::vector<Human*>& humans, std::vector<Enemy*>& enemies, float deltaTime) override;

private:
    Human* getNearestHuman(std::vector<Human*>& humans);

};


#endif
