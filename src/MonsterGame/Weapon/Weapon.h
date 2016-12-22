#ifndef FALCONLATINUM_WEAPON_H
#define FALCONLATINUM_WEAPON_H


#include <glm/vec2.hpp>
#include <vector>
#include "Bullet.h"

class Weapon
{
public:
    Weapon(int fireRate, float damage) : m_fireRate(fireRate), m_damage(damage), m_frame(0) {};
    //virtual void update(bool isMouseDown, const glm::vec2 position, const glm::vec2 direction, std::vector<Bullet>& bullets, float deltaTime) = 0;

protected:
    int m_fireRate;
    float m_damage;
    float m_frame;
};


#endif
