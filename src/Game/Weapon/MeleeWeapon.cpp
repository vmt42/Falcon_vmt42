#include <glm/gtx/rotate_vector.hpp>
#include <random>
#include "MeleeWeapon.h"

void
MeleeWeapon::update(bool isMouseDown, const glm::vec2 position, const glm::vec2 direction, std::vector<Slash> &slashes,
                    float deltaTime, float rotate)
{
    m_frame += 1.0f * deltaTime;

    if (m_frame >= m_fireRate && isMouseDown)
    {
        attack(position, direction, slashes, rotate);
        m_frame = 0;
    }
}

void MeleeWeapon::attack(const glm::vec2 position, const glm::vec2 direction, std::vector<Slash> &slashes, float rotate)
{
    slashes.emplace_back(m_damage, position, glm::rotate(direction, rotate));
}
