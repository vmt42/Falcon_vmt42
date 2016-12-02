#ifndef FALCONLATINUM_RANGEWEAPON_H
#define FALCONLATINUM_RANGEWEAPON_H


#include <glm/vec2.hpp>
#include <vector>
#include "Weapon.h"
#include "Bullet.h"

class RangeWeapon : public Weapon
{
public:
    RangeWeapon(int fireRate, float damage, float bulletSpeed, float bulletsPerShot, float spread)
            : Weapon(fireRate, damage), m_bulletSpeed(bulletSpeed), m_bulletsPerShot(bulletsPerShot), m_spread(spread){};

    void update(bool isMouseDown,const glm::vec2 position, const glm::vec2 direction, std::vector<Bullet>& bullets, float deltaTime);

protected:
    void fire(const glm::vec2 position, const glm::vec2 direction, std::vector<Bullet>& bullets);

protected:
    float m_spread;
    float m_bulletSpeed;
    float m_bulletsPerShot;
};


#endif
