#include "InputManager.h"

namespace Falcon
{


    void InputManager::pressKey(unsigned int keyID)
    {
        m_keyMap[keyID] = true;
    }

    void InputManager::releaseKey(unsigned int keyID)
    {
        m_keyMap[keyID] = false;
    }

    bool InputManager::isKeyPressed(unsigned int keyID) const
    {
        auto it = m_keyMap.find(keyID);

        if (it != m_keyMap.end())
            return it->second;
        else
            return false;
    }

    void InputManager::setMouseCoords(float x, float y)
    {
        m_mouseCoords.x = x;
        m_mouseCoords.y = y;
    }
}