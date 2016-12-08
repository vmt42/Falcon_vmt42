#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include "RangeWeapon.h"

void RangeWeapon::update(bool isMouseDown, const glm::vec2 position, const glm::vec2 direction, std::vector<Bullet> &bullets, float deltaTime)
{
    m_frame += 1.0f * deltaTime;

    if (m_frame >= m_fireRate && isMouseDown)
    {
        fire(position, direction, bullets);
        m_frame = 0;
    }
}

void RangeWeapon::fire(const glm::vec2 position, const glm::vec2 direction, std::vector<Bullet> &bullets)
{
    static std::mt19937 RNG;
    static std::uniform_real_distribution<float> randomRotate(-m_spread * 3.14159265359f / 180, m_spread * 3.14159265359f / 180);

    m_fire.play();
    for (int i = 0; i < m_bulletsPerShot; i++)
    {
        bullets.emplace_back(m_damage, position, glm::rotate(direction, randomRotate(RNG)), m_bulletSpeed);
    }
}
