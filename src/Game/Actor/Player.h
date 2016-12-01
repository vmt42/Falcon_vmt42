#ifndef FALCONLATINUM_PLAYER_H
#define FALCONLATINUM_PLAYER_H


#include "Human.h"
#include "../../InputOutput/InputManager.h"
#include "../../2D/Camera/Camera2D.h"
#include "../Weapon/Bullet.h"

class RangeWeapon;
class Player : public Human
{
public:
    Player() : m_currentGun(-1) {};
    ~Player() {};

    void init(glm::vec2 position, float speed, Falcon::InputManager* inputManager, Falcon::Camera2D* camera, std::vector<Bullet>* bullets);
    void draw(Falcon::SpriteBatch& spriteBatch) override;
    void update(const std::vector<std::string> &level, std::vector<Human*>& humans, std::vector<Enemy*>& enemies) override;

    void addRangeWeapon(RangeWeapon* rangeWeapon);

private:
    Falcon::InputManager* m_inputManager;
    Falcon::Camera2D* m_camera;
    std::vector<RangeWeapon*> m_rangeWeapons;
    std::vector<Bullet>* m_bullets;
    int m_currentGun;

};


#endif
