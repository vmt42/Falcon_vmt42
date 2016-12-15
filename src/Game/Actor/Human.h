#ifndef FALCONLATINUM_HUMAN_H
#define FALCONLATINUM_HUMAN_H


#include "Actor.h"

class Human : public Actor
{
public:
    Human() {};
    Human(glm::vec2 position, float speed);
    virtual ~Human() {};

    void draw(Falcon::SpriteBatch& spriteBatch) override;
    virtual void update(const std::vector<std::string> &level, std::vector<Human*>& humans, std::vector<Enemy*>& enemies, float deltaTime) override;

private:
    int m_frames;
};


#endif
