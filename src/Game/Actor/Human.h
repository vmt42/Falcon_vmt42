#ifndef FALCONLATINUM_HUMAN_H
#define FALCONLATINUM_HUMAN_H


#include "Actor.h"

class Human : public Actor
{
public:
    Human() : m_frames(0) {};
    virtual ~Human() {};

    void draw(Falcon::SpriteBatch& spriteBatch) override;
    void init(glm::vec2 position, float speed);
    virtual void update(const std::vector<std::string> &level, std::vector<Human*>& humans, std::vector<Enemy*>& enemies) override;

private:
    int m_frames;

    glm::vec2 m_direction;


};


#endif
