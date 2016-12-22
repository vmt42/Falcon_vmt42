#ifndef FALCONLATINUM_MELEEWEAPON_H
#define FALCONLATINUM_MELEEWEAPON_H


#include "Weapon.h"
#include "Slash.h"

class MeleeWeapon : public Weapon
{
public:
    MeleeWeapon(int fireRate, float damage) : Weapon(fireRate, damage) {};
    void update(bool isMouseDown,const glm::vec2 position, const glm::vec2 direction,
                std::vector<Slash> &slashes, float deltaTime);

private:
    void attack(const glm::vec2 position, const glm::vec2 direction, std::vector<Slash> &slashes);

private:
};


#endif