#ifndef FALCONLATINUM_PLAYER_H
#define FALCONLATINUM_PLAYER_H


#include "Human.h"
#include "../../InputOutput/InputManager.h"
#include "../../2D/Camera/Camera2D.h"
#include "../Weapon/Bullet.h"
#include "../Weapon/Slash.h"

class RangeWeapon;
class MeleeWeapon;

class Player : public Human
{
public:
    Player(glm::vec2 position, float speed, Falcon::InputManager* inputManager, Falcon::Camera2D* camera,
           std::vector<Bullet>* bullets, std::vector<Slash> *slash);
    ~Player() {};

    void draw(Falcon::SpriteBatch& spriteBatch) override;
    void update(const std::vector<std::string> &level, std::vector<Human*>& humans,
                std::vector<Enemy*>& enemies, float deltaTime) override;

    void addRangeWeapon(RangeWeapon* rangeWeapon);
    void addMeleeWeapon(MeleeWeapon* meleeWeapon);

    void addPoints(int points) { m_points += points; };
    int getPoints() { return m_points; };


private:
    int m_points;
    Falcon::InputManager* m_inputManager;
    Falcon::Camera2D* m_camera;
    std::vector<RangeWeapon*> m_rangeWeapons;
    MeleeWeapon* m_meleeWeapon;
    std::vector<Bullet>* m_bullets;
    std::vector<Slash>* m_slashes;
    int m_currentGun;

};


#endif
